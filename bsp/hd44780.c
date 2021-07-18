
/*****************************************************************************************************
* FILENAME :        hd44780.c
*
* DESCRIPTION :
*       File containing the APIs for managing the HD44780 module.
*
* PUBLIC FUNCTIONS :
*       void    hd44780_init(void)
*
* NOTES :
*       For further information about functions refer to the corresponding header file.
*
**/

#include "hd44780.h"
#include "stm32f446xx.h"
#include "gpio_driver.h"

/*****************************************************************************************************/
/*                                       Public API Definitions                                      */
/*****************************************************************************************************/

void hd44780_init(void){

    /* Configure the GPIO pins which are used for LCD connections */
    GPIO_Handle_t hd44780_signal;

    hd44780_signal.pGPIOx = HD44780_GPIO_PORT;
    hd44780_signal.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    hd44780_signal.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    hd44780_signal.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PULL;
    hd44780_signal.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    hd44780_signal.GPIO_PinConfig.GPIO_PinNumber = HD44780_GPIO_RS;
    GPIO_Init(&hd44780_signal);

    hd44780_signal.GPIO_PinConfig.GPIO_PinNumber = HD44780_GPIO_RW;
    GPIO_Init(&hd44780_signal);

    hd44780_signal.GPIO_PinConfig.GPIO_PinNumber = HD44780_GPIO_EN;
    GPIO_Init(&hd44780_signal);

    hd44780_signal.GPIO_PinConfig.GPIO_PinNumber = HD44780_GPIO_D4;
    GPIO_Init(&hd44780_signal);

    hd44780_signal.GPIO_PinConfig.GPIO_PinNumber = HD44780_GPIO_D5;
    GPIO_Init(&hd44780_signal);

    hd44780_signal.GPIO_PinConfig.GPIO_PinNumber = HD44780_GPIO_D6;
    GPIO_Init(&hd44780_signal);

    hd44780_signal.GPIO_PinConfig.GPIO_PinNumber = HD44780_GPIO_D7;
    GPIO_Init(&hd44780_signal);

    /* Set pins to 0 */
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_RS, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_RW, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_EN, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_D4, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_D5, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_D6, GPIO_PIN_RESET);
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_D7, GPIO_PIN_RESET);

    /* Do the HD44780 initialization */
}
