
/*****************************************************************************************************
* FILENAME :        hd44780.h
*
* DESCRIPTION :
*       Header file containing the prototypes of the APIs for HD44780 module.
*
* PUBLIC FUNCTIONS :
*       void    hd44780_init(void)
*
**/

#ifndef HD44780_H
#define HD44780_H

/**
 * Application configurable items
 */
#define HD44780_GPIO_PORT       GPIOD
#define HD44780_GPIO_RS         GPIO_PIN_NO_0
#define HD44780_GPIO_RW         GPIO_PIN_NO_1
#define HD44780_GPIO_EN         GPIO_PIN_NO_2
#define HD44780_GPIO_D4         GPIO_PIN_NO_3
#define HD44780_GPIO_D5         GPIO_PIN_NO_4
#define HD44780_GPIO_D6         GPIO_PIN_NO_5
#define HD44780_GPIO_D7         GPIO_PIN_NO_6

/*****************************************************************************************************/
/*                                       APIs Supported                                              */
/*****************************************************************************************************/

/**
 * @fn hd44780_init
 *
 * @brief function to initialize the hd44780 module.
 *
 * @param[in] void
 *
 * @return void
 */
void hd44780_init(void);

#endif /* HD44780_H */
