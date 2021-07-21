
/*****************************************************************************************************
* FILENAME :        hd44780.h
*
* DESCRIPTION :
*       Header file containing the prototypes of the APIs for HD44780 module.
*
* PUBLIC FUNCTIONS :
*       void    hd44780_init(void)
*       void    hd44780_send_command(uint8_t cmd)
*       void    hd44780_print_char(uint8_t data)
*       void    hd44780_print_string(char* msg)
*       void    hd44780_display_return_home(void)
*       void    hd44780_set_cursor(uint8_t row, uint8_t column)
*       void    hd44780_display_clear(void)
*
**/

#ifndef HD44780_H
#define HD44780_H

#include <stdint.h>

/**
 * Application configurable items
 */
#define HD44780_GPIO_PORT               GPIOC
#define HD44780_GPIO_RS                 GPIO_PIN_NO_8
#define HD44780_GPIO_RW                 GPIO_PIN_NO_9
#define HD44780_GPIO_EN                 GPIO_PIN_NO_11
#define HD44780_GPIO_D4                 GPIO_PIN_NO_10
#define HD44780_GPIO_D5                 GPIO_PIN_NO_4
#define HD44780_GPIO_D6                 GPIO_PIN_NO_5
#define HD44780_GPIO_D7                 GPIO_PIN_NO_6

/* HD44780 commands */
#define HD44780_CMD_4DL_2N_5X8F         0x28 /* 4 bit data length, 2 lines */
#define HD44780_CMD_DON_CURON           0x0E /* Display ON and cursor ON */
#define HD44780_CMD_DON_CUROFF          0x0C /* Display ON and cursor OFF */
#define HD44780_CMD_INCADD              0x06 /* Increment RAM address */
#define HD44780_CMD_DIS_CLEAR           0x01 /* Display clear */
#define HD44780_CMD_DIS_RETURN_HOME     0x02 /* Display return home */

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

/**
 * @fn hd44780_send_command
 *
 * @brief function to send a command to the HD44780.
 *
 * @param[in] cmd is the command to be sent.
 *
 * @return void
 */
void hd44780_send_command(uint8_t cmd);

/**
 * @fn hd44780_print_char
 *
 * @brief function to print a character in the HD44780.
 *
 * @param[in] data is the character to be sent.
 *
 * @return void
 *
 * @note: this function uses 4 bit parallel data transmission (D4, D5, D6 and D7 of HD44780).
 */
void hd44780_print_char(uint8_t data);

/**
 * @fn hd44780_print_string
 *
 * @brief function to print a string to the HD44780.
 *
 * @param[in] msg is the string to be printed.
 *
 * @return void
 */
void hd44780_print_string(char* msg);

/**
 * @fn hd44780_display_return_home
 *
 * @brief function to send the display return to home command to the HD44780.
 *
 * @param[in] void.
 *
 * @return void.
 */
void hd44780_display_return_home(void);

/**
 * @fn hd44780_set_cursor
 *
 * @brief function to set the cursor of the HD44780.
 *
 * @param[in] row to indicate the number of row (1 to 2).
 * @param[in] column to indicate the number of column (1 to 16).
 *
 * @return void.
 *
 * @note: this function is for a 2 x 16 characters display.
 */
void hd44780_set_cursor(uint8_t row, uint8_t column);

/**
 * @fn hd44780_display_clear
 *
 * @brief function to clear the display of the HD44780 device.
 *
 * @param[in] void.
 *
 * @return void.
 */
void hd44780_display_clear(void);

#endif /* HD44780_H */
