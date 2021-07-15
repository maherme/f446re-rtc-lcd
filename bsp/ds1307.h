/*****************************************************************************************************
* FILENAME :        ds1307.h
*
* DESCRIPTION :
*       Header file containing the prototypes of the APIs for DS1307 module.
*
* PUBLIC FUNCTIONS :
*       uint8_t ds1307_init(void)
*       void    ds1307_set_current_time(RTC_time_t* time)
*       void    ds1307_get_current_time(RTC_time_t* time)
*       void    ds1307_set_current_date(RTC_date_t* date)
*       void    ds1307_get_current_date(RTC_date_t* date)
*
**/

#ifndef DS1307_H
#define DS1307_H

#include <stdint.h>
#include "stm32f446xx.h"
#include "i2c_driver.h"
#include "gpio_driver.h"

/**
 * Application configurable items
 */
#define DS1307_I2C              I2C1
#define DS1307_I2C_GPIO_PORT    GPIOB
#define DS1307_I2C_SDA_PIN      GPIO_PIN_NO_7
#define DS1307_I2C_SCL_PIN      GPIO_PIN_NO_6
#define DS1307_I2C_SPEED        I2C_SCL_SPEED_SM
#define DS1307_I2C_PUPD         GPIO_PIN_PU

/**
 * Register addresses.
 */
#define DS1307_ADDR_SEC     0x00
#define DS1307_ADDR_MIC     0x01
#define DS1307_ADDR_HRS     0x02
#define DS1307_ADDR_DAY     0x03
#define DS1307_ADDR_DATE    0x04
#define DS1307_ADDR_MONTH   0x05
#define DS1307_ADDR_YEAR    0x06

/**
 * Time format.
 */
#define T_FORMAT_12HRS_AM   0
#define T_FORMAT_12HRS_PM   1
#define T_FORMAT_24HRS      2

/**
 * I2C address.
 */
#define DS1307_I2C_ADDR     0x68

/**
 * Days of the week.
 */
#define MONDAY              1
#define TUESDAY             2
#define WEDNESDAY           3
#define THURSDAY            4
#define FRIDAY              5
#define SATURDAY            6
#define SUNDAY              7

/**
 * Structure for storing date.
 */
typedef struct
{
    uint8_t date;
    uint8_t month;
    uint8_t year;
    uint8_t day;
}RTC_date_t;

/**
 * Structure for storing time.
 */
typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t time_format;
}RTC_time_t;

/*****************************************************************************************************/
/*                                       APIs Supported                                              */
/*****************************************************************************************************/

/**
 * @fn ds1307_init
 *
 * @brief function to initialize ds1307 module.
 *
 * @param[in] void
 *
 * @return TBC
 */
uint8_t ds1307_init(void);

/**
 * @fn ds1307_set_current_time
 *
 * @brief function to set time to ds1307 module.
 *
 * @param[in] RTC_time_t structure storing time to set.
 *
 * @return void
 */
void ds1307_set_current_time(RTC_time_t* time);

/**
 * @fn ds1307_get_current_time
 *
 * @brief function to get time for ds1307 module.
 *
 * @param[in] RTC_time_t structure for storing the time provided by the RTC module.
 *
 * @return void
 */
void ds1307_get_current_time(RTC_time_t* time);

/**
 * @fn ds1307_set_current_date
 *
 * @brief function to set date to ds1307 module.
 *
 * @param[in] RTC_date_t structure storing date to set.
 *
 * @return void
 */
void ds1307_set_current_date(RTC_date_t* date);

/**
 * @fn ds1307_get_current_date
 *
 * @brief function to get date for ds1307 module.
 *
 * @param[in] RTC_date_t structure for storing the date provided by the RTC module.
 *
 * @return void
 */
void ds1307_get_current_date(RTC_date_t* date);

#endif /* DS1307_H */
