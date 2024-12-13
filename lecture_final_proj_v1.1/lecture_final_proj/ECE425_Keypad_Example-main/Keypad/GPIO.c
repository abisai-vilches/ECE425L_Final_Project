/**
 * @file GPIO.c
 *
 * @brief Source code for the GPIO driver.
 *
 * This file contains the function definitions for the GPIO driver.
 * It interfaces with the following:
 *  - User LED (RGB) Tiva C Series TM4C123G LaunchPad
 *	- EduBase Board LEDs (LED0 - LED3)
 *	- EduBase Board Push Buttons (SW2 - SW5)
 *
 * To verify the pinout of the user LED, refer to the Tiva C Series TM4C123G LaunchPad User's Guide
 * Link: https://www.ti.com/lit/pdf/spmu296
 *
 * @author Aaron Nanas
 */

#include "GPIO.h"

// Constant definitions for the user LED (RGB) colors
const uint8_t RGB_LED_OFF 		= 0x00;
const uint8_t RGB_LED_RED 		= 0x02;
const uint8_t RGB_LED_BLUE 		= 0x04;
const uint8_t RGB_LED_GREEN 	= 0x08;

// Constant definitions for the EduBase board LEDs
const uint8_t EDUBASE_LED_ALL_OFF = 0x0;
const uint8_t EDUBASE_LED_ALL_ON	= 0xF;

void RGB_LED_Init(void)
{
	// Enable the clock to Port F
	SYSCTL->RCGCGPIO |= 0x20;

	// Set PF1, PF2, and PF3 as output GPIO pins
	GPIOF->DIR |= 0x0E;
	
	// Configure PF1, PF2, and PF3 to function as GPIO pins
	GPIOF->AFSEL &= ~0x0E;
	
	// Enable digital functionality for PF1, PF2, and PF3
	GPIOF->DEN |= 0x0E;
	
	// Initialize the output of the RGB LED to zero
	GPIOF->DATA &= ~0x0E;
}

void RGB_LED_Output(uint8_t led_value)
{
	// Set the output of the RGB LED
	GPIOF->DATA = (GPIOF->DATA & 0xF1) | led_value;
}

uint8_t RGB_LED_Status(void)
{
	// Assign the value of Port F to a local variable
	// and only read the values of the following bits: 3, 2, and 1
	// Then, return the local variable's value
	uint8_t RGB_LED_Status = GPIOF->DATA & 0x0E;
	return RGB_LED_Status;
}

void EduBase_LEDs_Init(void)
{
	// Enable the clock to Port B
	SYSCTL->RCGCGPIO |= 0x02;
	
	// Set PB0, PB1, PB2, and PB3 as output GPIO pins
	GPIOB->DIR |= 0x0F;
	
	// Configure PB0, PB1, PB2, and PB3 to function as GPIO pins
	GPIOB->AFSEL &= ~0x0F;
	
	// Enable digital functionality for PB0, PB1, PB2, and PB3
	GPIOB->DEN |= 0x0F;
	
	// Initialize the output of the EduBase LEDs to zero
	GPIOB->DATA &= ~0x0F;
}

void EduBase_LEDs_Output(uint8_t led_value)
{
	// Set the output of the LEDs
	GPIOB->DATA = (GPIOB->DATA & 0xF0) | led_value;
}

void EduBase_Button_Init(void)
{
	// Enable the clock to Port D
	SYSCTL->RCGCGPIO |= 0x08;
	
	// Set PD0, PD1, PD2, and PD3 as input GPIO pins
	GPIOD->DIR &= ~0x0F;
	
	// Configure PD0, PD1, PD2, and PD3 to function as GPIO pins
	GPIOD->AFSEL &= ~0x0F;
	
	// Enable digital functionality for PD0, PD1, PD2, and PD3
	GPIOD->DEN |= 0x0F;
}

uint8_t Get_EduBase_Button_Status(void)
{
	// Assign the value of Port D to a local variable
	// and only read the values of the following bits: 3, 2, 1, and 0
	// Then, return the local variable's value
	uint8_t button_status = GPIOD->DATA & 0x0F;
	return button_status;
}

void EduBase_Keypad_Init(void)
{
	// Enable the clock to GPIO Port A by setting the 
	// R0 bit (Bit 0) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x01;
	
	// Enable the clock to GPIO Port D by setting the 
	// R3 bit (Bit 3) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x08;
	
	// Configure the PA5, PA4, PA3, and PA2 pins as input
	// by clearing Bits 5 to 2 in the DIR register
	GPIOA->DIR &= ~0x3C;
	
	// Configure the PA5, PA4, PA3, and PA2 pins to function as
	// GPIO pins by clearing Bits 5 to 2 in the AFSEL register
	GPIOA->AFSEL &= ~0x3C;
	
	// Enable the digital functionality for the PA5, PA4, PA3, and PA2 pins
	// by setting Bits 5 to 2 in the DEN register
	GPIOA->DEN |= 0x3C;

	// Configure the PD3, PD2, PD1, and PD0 pins to function as
	// GPIO pins by clearing Bits 3 to 0 in the AFSEL register
	GPIOD->AFSEL &= ~0x0F;
	
	// Enable the digital functionality for the PD3, PD2, PD1, and PD0 pins
	// by setting Bits 3 to 0 in the DEN register
	GPIOD->DEN |= 0x0F;
}

uint8_t Get_EduBase_Keypad_Status(void)
{
	int row = 0;
	int col = 0;
	
	// Bit masks used for checking if one of the rows is active
	const uint8_t row_select[] = {0x01, 0x02, 0x04, 0x08};
	
	// Enable all rows
	GPIOD->DIR |= 0x0F;
	
	// Set the data row pins high
	GPIOD->DATA |= 0x0F;
	
	// Read all of the columns
	col = GPIOA->DATA & 0x3C;
	
	// Disable all of the rows
	GPIOD->DIR &= ~0x0F;
	
	// Return 0 if no key has been pressed
	if (col == 0) return 0;
	
	// Loop through each row to determine if a key has been pressed
	// It activates one row at a time and reads the input to see
	// if a specific column is active
	for (row = 0; row < 4; row++)
	{
		// Disable all rows
		GPIOD->DIR &= ~0x0F;
		
		// Enable the currently selected row
		GPIOD->DIR = GPIOD->DIR | row_select[row];
		
		// Turn the active row high and insert a short delay
		GPIOD->DATA |= 0x0F;
		SysTick_Delay1us(1);
		
		// Read all columns and if one of the inputs is low,
		// then one of the keys must be pressed
		col = GPIOA->DATA & 0x3C;
		if (col != 0) break;
	}
	
	GPIOD->DIR &= ~0x0F;
	if (row == 4) return 0;
	
	if (col == 0x04) return (row * 4 + 1); 			// Key in column 0
	else if (col == 0x08) return (row * 4 + 2); // Key in column 1
	else if (col == 0x10) return (row * 4 + 3); // Key in column 2
	else if (col == 0x20) return (row * 4 + 4); // Key in column 3
	else return 0;
}
