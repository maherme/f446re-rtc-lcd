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

/*****************************************************************************************************/
/*                                       Public API Definitions                                      */
/*****************************************************************************************************/

uint8_t ds1307_init(void){

    /* Initialize the I2C pins */
    ds1307_i2c_pin_cfg();

    /* Initialize the I2C peripheral */
    ds1307_i2c_cfg();

    return 0;
}

void ds1307_set_current_time(RTC_time_t* time){
}

void ds1307_get_current_time(RTC_time_t* time){
}

void ds1307_set_current_date(RTC_date_t* date){
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
