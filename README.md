# Stm32f446re with RTC and LCD
This is an embedded project based on [STM32F446RE](https://www.st.com/en/microcontrollers-microprocessors/stm32f446re.html) microcontroller which uses the [NUCLEO-F446RE](https://www.st.com/en/evaluation-tools/nucleo-f446re.html) board connected to the RTC (Real Time Clock) [DS1307](https://www.maximintegrated.com/en/products/analog/real-time-clocks/DS1307.html) and the LCD (Liquid Crystal Display) [Hitachi HD44780 16x2](https://pdf1.alldatasheet.es/datasheet-pdf/view/63673/HITACHI/HD44780.html).

## OpenOCD
You can use OpenOCD (Open On-Chip Debugger) for programming or debugging this project. You can starting OpenOCD typing:
```console
openocd -f board/st_nucleo_f4.cfg
```
Or using the Makefile:
```console
make load
```
You can use a telnet connection for connecting to the OpenOCD server:
```console
telnet 127.0.0.1 4444
```
You can program the microcontroller using:
```console
reset init
flash write_image erase your_app.elf
reset
```
Remember you must enable semihosting in the telnet session if you compile the project for using this feature (you can see printf outputs):
```console
arm semihosting enable
```
