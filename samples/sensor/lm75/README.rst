.. _lm75:

LM75 & Atmel AT30TS75A: High accuracy digital I2C temperature sensor
####################################################################

Description
***********

This sample application periodically (1Hz) measures the ambient temperature
in degrees Celsius. The result is written to the console.

The LM75 is an industry-standard digital temperature sensor with an 
integrated sigma-delta analog-to-digital converter (ADC) and I2C interface.

In this sample the Atmel AT30TS75A is used, which follows the LM75 standard.
Additional features of the AT30TS75A are:

 - User-configurable Resolution: 9 to 12 bits (0.5°C to 0.0625°C)
 - User-configurable High and Low Temperature Limits
 - ALERT Output Pin for Indicating Temperature Alarms

References
**********

 - AT30TS75A: https://www.microchip.com/en-us/product/AT30TS75A

Wiring
*******

This sample uses the AT30TS75A sensor controlled using the I2C interface.
Connect Supply: **VDD**, **GND** and Interface: **SDA**, **SCL**
and optionally connect the **INT** to a interrupt capable GPIO.
The supply voltage can be in the 2.7V to 5.5V range.
Depending on the baseboard used, the **SDA** and **SCL** lines require Pull-Up
resistors.

Building and Running
********************

This project outputs sensor data to the console. It requires an AT30TS75A 
(or any other LM75-type) sensor. Usage of the AT30TS75A sensor can be disabled
in main.c by setting `#define ATMEL_AT30TS75A 0`.
It should work with any platform featuring a I2C peripheral interface.
In this example below the :ref:`thingy52_nrf52832` board is used.


.. zephyr-app-commands::
   :zephyr-app: samples/sensor/lm75
   :board: thingy52_nrf52832
   :goals: build flash

Sample Output
=============

.. code-block:: console

   *** Booting Zephyr OS build zephyr-v3.0.0-3117-g1e02dd0dc195  ***
   device is 0x73e0, name is LM75
   [0:00:00.375]: temperature 22.000000 Cel
   [0:00:01.380]: temperature 22.062500 Cel
   [0:00:02.384]: temperature 22.062500 Cel
   [0:00:03.388]: temperature 22.062500 Cel
   [0:00:04.393]: temperature 22.062500 Cel

<repeats endlessly>
