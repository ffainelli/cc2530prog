# cc2530prog - Texas Instruments CC2530 Micro controller programming utility

## 1. General informations

This utility uses the CC2530 Debug Port to program the micro-controller. The
specific details of this interface are described in the following documents:
- [swru191b](http://www.ti.com/lit/swru191)
- [swra124](http://www.ti.com/lit/ug/swra124/swra124.pdf)

The hardware is programmed with the means of 3 GPIOs:
- reset (RST) (whose polarity can be software configured)
- data (DATA)
- clock (CLK)

The principle is the following:
- pulse the reset line to enter debug mode
- configure the hardware with DMA descriptors for transfering data
  from the DEBUG port directly to Flash
- clock out data to the debug port using the GPIOs

## 2. Software integration and modifications

The current version is provided using the Linux GPIO sysfs interface. Linux
can expose GPIOs to the user-space under **/sys/class/gpio** when the config
symbol **CONFIG_GPIO_SYSFS=y** is enabled.

3 GPIOs must be exposed by your hardware in order to use cc2530prog. In case
your system does not use GPIOs exposed through sysfs, you are supposed to
implement the following functions (also declared in gpio.h):

**gpio_export**: exports a GPIO pin for an user-space/consumer/producer application

**gpio_unexport**: unexports a GPIO pin

**gpio_set_direction**: set the direction (IN, OUT, HIGHZ) of the given pin

**gpio_get_value**: sets the output value of a given pin (pin must be output first)

**gpio_set_value**: returns the current gpio value

You are then supposed to set the Makefile environment **GPIO_BACKEND** to point
to the file implementing these GPIO routines for your specific platform.

## 3. Recommandations

The CC2530 firmware size matches the available hardware flash sizes (64KB up to
256KB) but since programming using the debug port is very slow, it is
recommended to bootstrap using the debug port and then use another mechanism to
transfer the bigger software image.

TI provides such a mechanism using an UART/SPI bootloader.

## 4. Future developments

At the moment nothing else is planned for this utility, however it should be
possible to use the routines exposed by this utility for doing interactive
debugging using the debug interface (which is what the CC2530 Debug dongles
basically do).

--
Florian Fainelli
