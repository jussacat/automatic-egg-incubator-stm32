/**
 *
 *                 Copyright (C) Noomane Ridha
 *
 * This source code is free to usefor any project
 *
 * \file keyboardMatrix.h
 *
 * \version 1.0
 *
 * \date 25 June 2020
 *
 * \brief
 *
 *
 * \section info Change Information
 *
 *
*/

#ifndef __KEYBOARD_MATRIX_H__
#define __KEYBOARD_MATRIX_H__

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================================*/
/*       I N C L U D E S                                                    */
/*==========================================================================*/

#if defined (USE_HAL_DRIVER)
#include "stm32f1xx_hal_gpio.h"					// HAL: stm32 HAL driver
#else
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#endif

/*==========================================================================*/
/*       G L O B A L   D A T A   D E C L A R A T I O N S                    */
/*==========================================================================*/

/*==========================================================================*/
/*       L O C A L   D A T A   D E C L A R A T I O N S                      */
/*==========================================================================*/

/*==========================================================================*/
/*       L O C A L   F U N C T I O N S   P R O T O T Y P E S                */
/*==========================================================================*/


/*==========================================================================*/
/*       G L O B A L  F U N C T I O N S                                     */
/*==========================================================================*/							
										 
/*!
 * @brief Keypad Init all rows GPIO and Columns GPIO
 *
 * @param none
 * @return none
 */												 
void keyboardInit(void);

/*!
 * @brief return key pressed.
 *
 * @param none
 * @return char
 */
char getKeyPressed(void);

#ifdef __cplusplus
}
#endif												 
												 
#endif /* */
