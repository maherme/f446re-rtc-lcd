/*****************************************************************************************************
* FILENAME :        main.c
*
* DESCRIPTION :
*       File containing the main function.
*
**/

#include <stdio.h>
#include <stdint.h>
#include "ds1307.h"

#define SYSTICK_TIM_CLK     16000000UL

extern void initialise_monitor_handles(void);

/**
 * @fn get_day_week
 *
 * @brief function to convert the number of the day of the week into the name.
 *
 * @param[in] number of the day of the week.
 *
 * @return string with the name of the day of the week.
 */
static char* get_day_week(uint8_t day){

    char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    return days[day-1];
}

/**
 * @fn number_to_str
 *
 * @brief function to convert a number into a string.
 *
 * @param[in] number in integer 8bits format.
 * @param[out] string with the converted number.
 *
 * @return void.
 */
static void number_to_str(uint8_t num, char* buf){

    if(num < 10){
        buf[0] = '0';
        buf[1] = num + 48; /* Conver to ASCII */
    }
    else if((num >= 10) && (num < 99)){
        buf[0] = (num / 10) + 48;
        buf[1] = (num % 10) + 48;
    }
    else{
        /* do nothing */
    }
}

/**
 * @fn time_to_str
 *
 * @brief function to create a string with time values.
 *
 * @param[in] time struct with the time values.
 *
 * @return string with the time values in format hh:mm:ss.
 */
static char* time_to_str(RTC_time_t* time){

    static char buf[9] = {0};

    buf[2] = ':';
    buf[5] = ':';

    number_to_str(time->hours, buf);
    number_to_str(time->minutes, &buf[3]);
    number_to_str(time->seconds, &buf[6]);

    buf[8] = '\0';

    return buf;
}

/**
 * @fn date_to_str
 *
 * @brief function to create a string with date values.
 *
 * @param[in] date struct with the date values.
 *
 * @return string with the date values in format dd/mm/yy.
 */
static char* date_to_str(RTC_date_t* date){

    static char buf[9] = {0};

    buf[2] = '/';
    buf[5] = '/';

    number_to_str(date->date, buf);
    number_to_str(date->month, &buf[3]);
    number_to_str(date->year, &buf[6]);

    buf[8] = '\0';

    return buf;
}

/**
 * @fn init_systick_timer
 *
 * @brief function to initialize the systick timer peripheral.
 *
 * @param[in] frequency in Hz.
 *
 * @return void.
 */
static void init_systick_timer(uint32_t tick_hz){

    uint32_t count_value = 0;
    uint32_t* pSRVR = (uint32_t*)0xE000E014;
    uint32_t* pSCSR = (uint32_t*)0xE000E010;

    /* Calculation of reload value */
    count_value = (SYSTICK_TIM_CLK / tick_hz) - 1;

    /* Clear the value of SVR */
    *pSRVR &= ~(0xFFFFFFFF);

    /* Load the value into SVR */
    *pSRVR |= count_value;

    /* Enable systick exception request */
    *pSCSR |= (1 << 1);
    /* Indicate clock source, processor clock source */
    *pSCSR |= (1 << 2);

    /* Enable the systick */
    *pSCSR |= (1 << 0);
}

int main(void){

    RTC_time_t current_time;
    RTC_date_t current_date;
    char* am_pm = NULL;

    initialise_monitor_handles();

    printf("Starting program!!!\n");

    if(ds1307_init()){
        printf("RTC init failed, please reset manually\n");
        while(1);
    }

    /* Initialize the systick timer */
    init_systick_timer(1);

    /* Configure date */
    current_date.day = SATURDAY;
    current_date.date = 17;
    current_date.month = 7;
    current_date.year = 21;

    /* Configure time */
    current_time.hours = 11;
    current_time.minutes = 59;
    current_time.seconds = 15;
    current_time.time_format = T_FORMAT_12HRS_PM;

    /* Set date and time into DS1307 */
    ds1307_set_current_date(&current_date);
    ds1307_set_current_time(&current_time);

    /* Get date and time from DS1307 */
    ds1307_get_current_date(&current_date);
    ds1307_get_current_time(&current_time);

    /* Print time */
    if(current_time.time_format != T_FORMAT_24HRS){
        am_pm = current_time.time_format ? "PM" : "AM";
        printf("Current time: %s %s\n", time_to_str(&current_time), am_pm); /* Format hh:mm:ss <A/P>M */
    }
    else{
        printf("Current time: %s\n", time_to_str(&current_time)); /* Format hh:mm:ss */
    }

    /* Print date */
    printf("Current date: %s <%s>\n", date_to_str(&current_date), get_day_week(current_date.day));

    for(;;){
    }

    return 0;
}

void Systick_Handler(void){

    RTC_date_t current_date;
    RTC_time_t current_time;
    char* am_pm = NULL;

    /* Get time */
    ds1307_get_current_time(&current_time);
    /* Print time */
    if(current_time.time_format != T_FORMAT_24HRS){
        am_pm = current_time.time_format ? "PM" : "AM";
        printf("Current time: %s %s\n", time_to_str(&current_time), am_pm); /* Format hh:mm:ss <A/P>M */
    }
    else{
        printf("Current time: %s\n", time_to_str(&current_time)); /* Format hh:mm:ss */
    }

    /* Get date */
    ds1307_get_current_date(&current_date);
    /* Print date */
    printf("Current date: %s <%s>\n", date_to_str(&current_date), get_day_week(current_date.day));
}
