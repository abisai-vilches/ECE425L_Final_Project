/*
 * @file helpers.c
 *
 * @brief Helper codes for the Tic Tac Toe program.
 *
 * This file contains the functions used in the main.c program to 
 * dispaly the board and check the winner
 *
 * @author Abisai Vilches, Brandon Vargas
 */
 
#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
#include "UART0.h"
#include "helpers.h"

void Display_Calculator(char disp[4][4])
{
	UART0_Output_String("Keys of the Calculator");
	UART0_Output_Newline();
    for (int i = 0; i < 4; i++) 
		{
        for (int j = 0; j < 4; j++) 
				{
					UART0_Output_Character(' ');
					UART0_Output_Character(disp[i][j]);
           if (j < 4 - 1) 
					{
               UART0_Output_String(" |");
          }
        }
        UART0_Output_Newline();
        if (i < 4 - 1) 
				{
          UART0_Output_String("---------------");
					UART0_Output_Newline();
        }
   }
}
