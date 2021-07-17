/*****************************************************************************************************
* FILENAME :        main.c
*
* DESCRIPTION :
*       File containing the main function.
*
**/

#include <stdio.h>

extern void initialise_monitor_handles(void);

int main(void){

    initialise_monitor_handles();

    printf("Starting program!!!\n");

    for(;;){
    }

    return 0;
}
