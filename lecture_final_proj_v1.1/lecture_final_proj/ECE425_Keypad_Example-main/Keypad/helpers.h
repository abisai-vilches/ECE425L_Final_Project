/**
 * @file helpers.h
 *
 * @brief Header file for the helper functions in helpers.c.
 *
 * This file contains the function definitions for the following:
 *  - Display_Board
 *  - Check_Win
 *
 * @author Aaron Nanas
 */

#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
#include "UART0.h"

/**
 * @brief The Display_Board function displays the tic tac toe board.
 *
 * This function uses for loops and UART display functions to output
 * the board and its cell contents on the screen.
 *
 * @param board		A 3 by 3 array that contains the contents of each cell
 *								of the board
 *
 * @return None
 */
 void Display_Calculator(char disp[4][4]);
 