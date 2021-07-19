
/*****************************************************************************************************
* FILENAME :        hd44780.c
*
* DESCRIPTION :
*       File containing the APIs for managing the HD44780 module.
*
* PUBLIC FUNCTIONS :
*       void    hd44780_init(void)
*       void    hd44780_send_command(uint8_t cmd)
*       void    hd44780_print_char(uint8_t data)
*       void    hd44780_print_string(char* msg)
*       void    hd44780_display_return_home(void)
*       void    hd44780_set_cursor(uint8_t row, uint8_t column)
*
* NOTES :
*       For further information about functions refer to the corresponding header file.
*
**/

#include "hd44780.h"
#include "stm32f446xx.h"
#include "gpio_driver.h"
#include <stdint.h>
#include <string.h>

/*****************************************************************************************************/
/*                                       Static Function Prototypes                                  */
/*****************************************************************************************************/

/**
 * @fn write_4_bits
 *
 * @brief function to write four bits in the data line of HD44780.
 *
 * @param[in] value to be written in the data line.
 *
 * @return void.
 */
static void write_4_bits(uint8_t value);

/**
 * @fn hd44780_enable
 *
 * @brief function to enable the HD44780 device.
 *
 * @param[in] void.
 *
 * @return void.
 */
static void hd44780_enable(void);

/**
 * @fn hd44780_display_clear
 *
 * @brief function to clear the display of the HD44780 device.
 *
 * @param[in] void.
 *
 * @return void.
 */
static void hd44780_display_clear(void);

/**
 * @fn mdelay
 *
 * @brief function to count a number of miliseconds.
 *
 * @param[in] cnt is the number of miliseconds.
 *
 * @return void.
 */
static void mdelay(uint32_t cnt);

/**
 * @fn udelay
 *
 * @brief function to count a number of microseconds.
 *
 * @param[in] cnt is the number of microseconds.
 *
 * @return void.
 */
static void udelay(uint32_t cnt);

/*****************************************************************************************************/
/*                                       Public API Definitions                                      */
/*****************************************************************************************************/

void hd44780_init(void){

    /* Configure the GPIO pins which are used for LCD connections */
    GPIO_Handle_t hd44780_signal;

    memset(&hd44780_signal, 0, sizeof(hd44780_signal));

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
    mdelay(40);

    /* Set RS to 0 for HD44780 command */
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_RS, GPIO_PIN_RESET);

    /* Set RW to 0, Writing to HD44780 */
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_RW, GPIO_PIN_RESET);

    write_4_bits(0x03);

    mdelay(5);

    write_4_bits(0x03);

    udelay(150);

    write_4_bits(0x03);
    write_4_bits(0x02);

    /* Set command */
    hd44780_send_command(HD44780_CMD_4DL_2N_5X8F);

    /* Display ON and cursor ON */
    hd44780_send_command(HD44780_CMD_DON_CURON);

    /* Display clear */
    hd44780_display_clear();

    /* Entry mode set */
    hd44780_send_command(HD44780_CMD_INCADD);
}

void hd44780_send_command(uint8_t cmd){

    /* Set RS to 0 for HD44780 command */
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_RS, GPIO_PIN_RESET);

    /* Set RW to 0 for writing */
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_RW, GPIO_PIN_RESET);

    /* Send higher nibble */
    write_4_bits(cmd >> 4);
    /* Send lower nibble */
    write_4_bits(cmd & 0x0F);
}

void hd44780_print_char(uint8_t data){

    /* Set RS to 1 for HD44780 user data */
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_RS, GPIO_PIN_SET);

    /* Set RW to 0 for writing */
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_RW, GPIO_PIN_RESET);

    /* Send higher nibble */
    write_4_bits(data >> 4);
    /* Send lower nibble */
    write_4_bits(data & 0x0F);
}

void hd44780_print_string(char* msg){

    do{
        hd44780_print_char((uint8_t)*msg++);
    }
    while(*msg != '\0');
}

void hd44780_display_return_home(void){

    /* Send display return to home command */
    hd44780_send_command(HD44780_CMD_DIS_RETURN_HOME);

    /* Wait */
    mdelay(2);
}

void hd44780_set_cursor(uint8_t row, uint8_t column){

    column--;

    switch(row){
        case 1:
            /* Set cursor to first row address and add index */
            hd44780_send_command(column |= 0x80);
            break;
        case 2:
            /* Set cursor to second row address and add index */
            hd44780_send_command(column |= 0xC0);
            break;
        default:
            /* do nothing */
            break;
    }
}

/*****************************************************************************************************/
/*                                       Static Function Definitions                                 */
/*****************************************************************************************************/

static void write_4_bits(uint8_t value){

    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_D4, ((value >> 0) & 0x1));
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_D5, ((value >> 1) & 0x1));
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_D6, ((value >> 2) & 0x1));
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_D7, ((value >> 3) & 0x1));

    hd44780_enable();
}

static void hd44780_enable(void){

    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_EN, GPIO_PIN_SET);
    udelay(10);
    GPIO_WriteToOutputPin(HD44780_GPIO_PORT, HD44780_GPIO_EN, GPIO_PIN_RESET);
    udelay(100);
}

static void hd44780_display_clear(void){

    /* Send display clear command */
    hd44780_send_command(HD44780_CMD_DIS_CLEAR);

    /* Wait */
    mdelay(2);
}

static void mdelay(uint32_t cnt){

    uint32_t i = 0;

    for(i = 0; i < (cnt * 1000); i++);
}

static void udelay(uint32_t cnt){

    uint32_t i = 0;

    for(i = 0; i < cnt; i++);
}
