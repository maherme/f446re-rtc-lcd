/*****************************************************************************************************
* FILENAME :        ds1307.c
*
* DESCRIPTION :
*       File containing the APIs for managing the DS1307 module.
*
* PUBLIC FUNCTIONS :
*       uint8_t ds1307_init(void)
*       void    ds1307_set_current_time(RTC_time_t* time)
*       void    ds1307_get_current_time(RTC_time_t* time)
*       void    ds1307_set_current_date(RTC_date_t* date)
*       void    ds1307_get_current_date(RTC_date_t* date)
*
* NOTES :
*       For further information about functions refer to the corresponding header file.
*
**/

#include "ds1307.h"
#include "gpio_driver.h"
#include "i2c_driver.h"
#include <stdint.h>
#include <string.h>

I2C_Handle_t ds1307_I2CHandle;

/*****************************************************************************************************/
/*                                       Static Function Prototypes                                  */
/*****************************************************************************************************/

/**
 * @fn ds1307_i2c_pin_cfg
 *
 * @brief helper function to configure the GPIO pins for I2C peripheral.
 *
 * @param[in] void.
 *
 * @return void.
 */
static void ds1307_i2c_pin_cfg(void);

/**
 * @fn ds1307_i2c_cfg
 *
 * @brief helper function to configure the I2C peripheral.
 *
 * @param[in] void.
 *
 * @return void.
 */
static void ds1307_i2c_cfg(void);

/**
 * @fn ds1307_write
 *
 * @brief helper function to write the register of DS1307 device.
 *
 * @param[in] value is the value to be stored in the register
 * @param[in] reg_addr is the value of the register address.
 *
 * @return void.
 */
static void ds1307_write(uint8_t value, uint8_t reg_addr);

/**
 * @fn ds1307_read
 *
 * @brief helper function to read the register of DS1307 device.
 *
 * @param[in] reg_addr is the value of the register address.
 *
 * @return value of the register.
 */
static uint8_t ds1307_read(uint8_t reg_addr);

/*****************************************************************************************************/
/*                                       Public API Definitions                                      */
/*****************************************************************************************************/

uint8_t ds1307_init(void){

    uint8_t clock_state = 0;

    /* Initialize the I2C pins */
    ds1307_i2c_pin_cfg();

    /* Initialize the I2C peripheral */
    ds1307_i2c_cfg();

    /* Enable the I2C peripheral */
    I2C_Enable(DS1307_I2C ,ENABLE);

    /* Make clock halt = 0 */
    ds1307_write(0x00, DS1307_ADDR_SEC);

    /* Read back clock halt bit */
    clock_state = ds1307_read(DS1307_ADDR_SEC);

    return ((clock_state >> 7) & 0x01);
}

void ds1307_set_current_time(RTC_time_t* time){

    uint8_t seconds = 0;
    uint8_t hours = 0;

    seconds = bin_to_bcd(time->seconds);
    /* Clear CH bit */
    seconds &= ~(1 << 7);
    /* Program seconds value */
    ds1307_write(seconds, DS1307_ADDR_SEC);
    /* Program minutes value */
    ds1307_write(bin_to_bcd(time->minutes), DS1307_ADDR_MIN);
    /* Program hours value */
    hours = bin_to_bcd(time->hours);
    if(time->time_format == T_FORMAT_24HRS){
        hours &= ~(1 << 6);
    }
    else{
        hours |= (1 << 6);
        hours = (time->time_format == T_FORMAT_12HRS_PM) ? hours | (1 << 5) : hours & ~(1 << 5);
    }
    ds1307_write(hours, DS1307_ADDR_HRS);
}

void ds1307_get_current_time(RTC_time_t* time){

    uint8_t seconds = 0;
    uint8_t hours = 0;

    /* Get seconds */
    seconds = ds1307_read(DS1307_ADDR_SEC);
    seconds &= (1 << 7);
    time->seconds = bcd_to_bin(seconds);
    /* Get minutes */
    time->minutes = bcd_to_bin(ds1307_read(DS1307_ADDR_MIN));
    /* Get hours */
    hours = ds1307_read(DS1307_ADDR_HRS);
    if(hours & (1 << 6)){
        /* 12 hrs format */
        time->time_format = !((hours & (1 << 5)) == 0);
        /* Clear bits 5 and 6 */
        hours &= ~(0x03 << 5);
    }
    else{
        /* 24 hrs format */
        time->time_format = T_FORMAT_24HRS;
    }
    time->hours = bcd_to_bin(hours);
}

void ds1307_set_current_date(RTC_date_t* date){

    /* Set date register */
    ds1307_write(bin_to_bcd(date->date), DS1307_ADDR_DATE);
    /* Set month register */
    ds1307_write(bin_to_bcd(date->month), DS1307_ADDR_MONTH);
    /* Set year register */
    ds1307_write(bin_to_bcd(date->year), DS1307_ADDR_YEAR);
    /* Set day register */
    ds1307_write(bin_to_bcd(date->day), DS1307_ADDR_DAY);
}

void ds1307_get_current_date(RTC_date_t* date){
}

/*****************************************************************************************************/
/*                                       Static Function Definitions                                 */
/*****************************************************************************************************/

static void ds1307_i2c_pin_cfg(void){

    GPIO_Handle_t i2c_sda, i2c_scl;

    memset(&i2c_sda, 0, sizeof(i2c_sda));
    memset(&i2c_scl, 0, sizeof(i2c_scl));

    i2c_sda.pGPIOx = DS1307_I2C_GPIO_PORT;
    i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
    i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    i2c_sda.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SDA_PIN;
    i2c_sda.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
    i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    GPIO_Init(&i2c_sda);

    i2c_scl.pGPIOx = DS1307_I2C_GPIO_PORT;
    i2c_scl.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
    i2c_scl.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    i2c_scl.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SCL_PIN;
    i2c_scl.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
    i2c_scl.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    GPIO_Init(&i2c_scl);
}

static void ds1307_i2c_cfg(void){

    ds1307_I2CHandle.pI2Cx = DS1307_I2C;
    ds1307_I2CHandle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
    ds1307_I2CHandle.I2C_Config.I2C_SCLSpeed = DS1307_I2C_SPEED;

    I2C_Init(&ds1307_I2CHandle);
}

static void ds1307_write(uint8_t value, uint8_t reg_addr){

    uint8_t tx[2] = {0};

    tx[0] = reg_addr;
    tx[1] = value;

    I2C_MasterSendData(&ds1307_I2CHandle, tx, sizeof(tx), DS1307_I2C_ADDR, I2C_DISABLE_SR);
}

static uint8_t ds1307_read(uint8_t reg_addr){

    uint8_t data = 0;

    I2C_MasterSendData(&ds1307_I2CHandle, &reg_addr, 1, DS1307_I2C_ADDR, I2C_DISABLE_SR);
    I2C_MasterReceiveData(&ds1307_I2CHandle, &data, 1, DS1307_I2C_ADDR, I2C_DISABLE_SR);

    return data;
}
