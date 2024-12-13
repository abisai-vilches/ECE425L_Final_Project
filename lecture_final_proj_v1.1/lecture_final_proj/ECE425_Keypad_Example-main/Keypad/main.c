/*
 * @file main.c
 *
 * @brief Main source code for the Keypad program.
 *
 * This file contains the main entry point and function definitions for the Keypad program.
 *
 * @author Aaron Nanas
 */

#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
#include "GPIO.h"
#include "UART0.h"
#include "helpers.h"

int Num_Decode_EduBase_Keypad(uint8_t edubase_keypad_status, int num);
char Op_Decode_EduBase_Keypad(uint8_t edubase_keypad_status, char op);

static int reset = 0;
static char input = '0';

int main(void)
{
	SysTick_Delay_Init();
	
	EduBase_LEDs_Init();
	
	EduBase_Keypad_Init();
	
	UART0_Init();
	
	int num1 = 0;
	int num2 = 0;
	char operation = 0;
	
	char cells[4][4] = {{'1', '2', '3', '+'}, {'4', '5', '6', '-'}, {'7', '8', '9', 'x'}, {'*', '0', '#', '/'}};
	
	uint8_t EduBase_Keypad_Status = Get_EduBase_Keypad_Status();
	
while(1)
{
	while(reset == 0)
	{
		UART0_Output_Newline();
		Display_Calculator(cells);
		UART0_Output_Newline();
		
		UART0_Output_String("Enter the first number using numbers on the keypad");
		UART0_Output_Newline();
		UART0_Output_String("Press # to submit, or * to clear");
		UART0_Output_Newline();
		
		// Get the first number (num1)
		while (input != '#') // when input is #, the number is submitted
		{
			EduBase_Keypad_Status = Get_EduBase_Keypad_Status();
			EduBase_LEDs_Output(EduBase_Keypad_Status);
			num1 = Num_Decode_EduBase_Keypad(EduBase_Keypad_Status, num1);
		}
		
		if (reset == 1)
		{
			UART0_Output_Newline();
			UART0_Output_String("Resetting...");
			SysTick_Delay1ms(500);
			UART0_Output_Newline();
			
			input = '0';
			num1 = 0;
			num2 = 0;
			operation = '0';
			break;
		}
		
		input = '0'; // reset input to 0 so we can take the next input
		
		UART0_Output_Newline();
		UART0_Output_String("First Number = ");
		UART0_Output_Unsigned_Decimal(num1);
		
		UART0_Output_Newline();
		UART0_Output_String("Enter the operation using the keypad");
		UART0_Output_Newline();
		UART0_Output_String("Press * to clear");
		UART0_Output_Newline();
		
		// Get the operation
		while (input != '#') // the operation can be changed until # is pressed. operation only saves the last character before # is pressed
		{
			EduBase_Keypad_Status = Get_EduBase_Keypad_Status();
			EduBase_LEDs_Output(EduBase_Keypad_Status);
			operation = Op_Decode_EduBase_Keypad(EduBase_Keypad_Status, operation);
		}
		
		if (reset == 1)
		{
			UART0_Output_Newline();
			UART0_Output_String("Resetting...");
			SysTick_Delay1ms(500);
			UART0_Output_Newline();
			
			input = '0';
			num1 = 0;
			num2 = 0;
			operation = '0';
			break;
		}
		
		input = '0'; // reset input to 0 so we can take the next input
		
		UART0_Output_Newline();
		UART0_Output_String("Operation: ");
		UART0_Output_Character(operation);
		
		UART0_Output_Newline();
		UART0_Output_String("Enter the second number using numbers on the keypad");
		UART0_Output_Newline();
		UART0_Output_String("Press # to submit, or * to clear");
		UART0_Output_Newline();
		
		// Get the second number (num2)
		while (input != '#')
		{
			EduBase_Keypad_Status = Get_EduBase_Keypad_Status();
			EduBase_LEDs_Output(EduBase_Keypad_Status);
			num2 = Num_Decode_EduBase_Keypad(EduBase_Keypad_Status, num2);
		}
		
		if (reset == 1)
		{
			UART0_Output_Newline();
			UART0_Output_String("Resetting...");
			SysTick_Delay1ms(500);
			UART0_Output_Newline();
			
			input = '0';
			num1 = 0;
			num2 = 0;
			operation = '0';
			break;
		}
		
		// Arithmetic operation
		int result = 0; 
						
				// Perform operation
				switch(operation)
				{
					case '+':
					{
						result = num1 + num2;
						UART0_Output_Newline();
            UART0_Output_String("Result = ");
            UART0_Output_Unsigned_Decimal(result);
            UART0_Output_Newline();
						break;
					}
					case '-':
					{
						// Cannot do negative numbers so it will ask for a diffent value if num1 > num2
						if(num1 >= num2)
						{
							result = num1 - num2;
							UART0_Output_Newline();
							UART0_Output_String("Result = ");
							UART0_Output_Unsigned_Decimal(result);
							UART0_Output_Newline();
						}
						else
						{
							UART0_Output_Newline();
							result = num2 - num1;
							UART0_Output_Newline();
							UART0_Output_String("Result = -");
							UART0_Output_Unsigned_Decimal(result);
							UART0_Output_Newline();
						}
						break;
					}
					case 'x':
					{
						result = num1 * num2;
						UART0_Output_Newline();
            UART0_Output_String("Result = ");
            UART0_Output_Unsigned_Decimal(result);
            UART0_Output_Newline();
						break;
					}
					case '/':
					{
						if(num2 != 0)
						{
							result = num1 / num2;
							UART0_Output_Newline();
							UART0_Output_String("Result = ");
							UART0_Output_Unsigned_Decimal(result);
							UART0_Output_Newline();
						}
						else
						{
							UART0_Output_Newline();
							UART0_Output_String("Invalid Operation");
							UART0_Output_Newline();
						}
						break;
					}
					default:
					{
						UART0_Output_String("Invalid Operation");
						UART0_Output_Newline();
						continue;
					}
				}
			

				// Reset number
				num1 = 0;
        num2 = 0;
        operation = '0';
        input = '0';  // Reset input
				result = 0;
				reset = 0;
			}
		reset = 0;
	}
}

int Num_Decode_EduBase_Keypad(uint8_t edubase_keypad_status, int num)
{
	// This function only has the cases for the keys that represent numbers, as well as clear and submit
	// Pressing any operator keys (A, B, C, D) does nothing
	switch(edubase_keypad_status) 
	{		
		case 1:
		{
			UART0_Output_String("1");
			num = num*10 + 1;
			SysTick_Delay1ms(500);
			return num;
		}
	
		case 2:
		{
			UART0_Output_String("2");
			num = num*10 + 2;
			SysTick_Delay1ms(500);
			return num;
		}
	
		case 3:
		{
			UART0_Output_String("3");
			num = num*10 + 3;
			SysTick_Delay1ms(500);
			return num;
		}
	
		case 5:
		{
			UART0_Output_String("4");
			num = num*10 + 4;
			SysTick_Delay1ms(500);
			return num;
		}
	
		case 6:
		{
			UART0_Output_String("5");
			num = num*10 + 5;
			SysTick_Delay1ms(500);
			return num;
		}
	
		case 7:
		{
			UART0_Output_String("6");
			num = num*10 + 6;
			SysTick_Delay1ms(500);
			return num;
		}
	
		case 9:
		{
			UART0_Output_String("7");
			num = num*10 + 7;
			SysTick_Delay1ms(500);
			return num;
		}
	
		case 10:
		{
			UART0_Output_String("8");
			num = num*10 + 8;
			SysTick_Delay1ms(500);
			return num;
		}
	
		case 11:
		{
			UART0_Output_String("9");
			num = num*10 + 9;
			SysTick_Delay1ms(500);
			return num;
		}
	
		case 13:
		{
			UART0_Output_Newline();
			input = '#';
			UART0_Output_String("clear");
			num = 0;
			reset = 1;
			SysTick_Delay1ms(500);
			break;
		}
	
		case 14:
		{
			UART0_Output_String("0");
			num = num*10;
			SysTick_Delay1ms(500);
			return num;
		}
	
		case 15:
		{
			UART0_Output_Newline();
			UART0_Output_String("enter");
			input = '#';
			SysTick_Delay1ms(500);
			break;
		}
	
	}
	return num;
}

char Op_Decode_EduBase_Keypad(uint8_t edubase_keypad_status, char op)
{
	// This function only has the cases for the keys that represent operations, as well as clear and submit
	// Pressing any numbers keys does nothing
	switch(edubase_keypad_status)
	{		
		case 4:
		{
			UART0_Output_String("+");
			SysTick_Delay1ms(500);
			op = '+';
			break;
		}
	
		case 8:
		{
			UART0_Output_String("-");
			SysTick_Delay1ms(500);
			op =  '-';
			break;
		}
		
		case 12:
		{
			UART0_Output_String("x");
			SysTick_Delay1ms(500);
			op = 'x';
			break;
		}
	
		case 13:
		{
			UART0_Output_Newline();
			input = '#';
			UART0_Output_String("clear");
			reset = 1;
			SysTick_Delay1ms(500);
			op = '0';
			break;
		}
	
		case 15:
		{
			UART0_Output_Newline();
			UART0_Output_String("enter");
			input = '#';
			SysTick_Delay1ms(500);
			break;
		}
		
		case 16:
		{
			UART0_Output_String("/");
			SysTick_Delay1ms(500);
			op = '/';
			break;
		}
	}
	return op;
}