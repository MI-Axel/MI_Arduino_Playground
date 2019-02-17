# Curiosity MotorShield board Introduction

The original MuchInnovation Motorshield kit is one of our most beloved kits, which is why we decided to make something even better. We have upgraded the shield kit to make the bestest, easiest way to drive DC, Servo and Stepper motors. This shield will make quick work of your next robotics and racing car project! We made the ability to drive up to 2 DC motors or 1 stepper motors with 16 channel servo motors!

## DESCRIPTION

Instead of a L293D darlington driver, we now have the DRV8848 MOSFET drivers with 1A per channel current capability (you can draw up to 3A peak for approx 20ms at a time). It also has much lower voltage drops across the motor so you get more torque out of your batteries, and there are built-in flyback diodes as well.

Instead of using a latch and the Arduino's PWM pins, we have a fully-dedicated PWM driver chip onboard. This chip handles all the servo motor position controls over I2C. Only two data pins (SDA & SCL in addition to the power pins GND & 5V) are required to drive the up to 16 channels of motors, and since it's I2C you can also connect any other I2C devices or shields to the same pins. This also makes it drop-in compatible with any Arduino, such as the Uno, Due, Leonardo and Mega R3.

**Completely stackable design:** 5 address-select pins means up to 32 stackable shields: that's 64 DC motors or 32 steepper motors with 512 channels of servo motors! What on earth could you do with that many motors? I have no idea but if you come up with something send us a photo because that would be a pretty glorious project.

Lots of other small improvements such as a polarity protection diode on the power input pins. And the shield is assembled and tested here at MuchInnovation so all you have to do is stacking headers and the terminal blocks.

Lets check out these specs again:

* 2 connections for 5V 'hobby' servos connected to the Arduino's high-resolution dedicated timer - no jitter!
* 2 H-Bridges: DRV8848 chipset provides 1A per bridge (3A for brief period peaks) with thermal shutdown protection, internal kickback protection diodes. Can run motors on 4.5VDC to 13.5VDC.
* Sopport 2 bi-directional DC motors with individual 8-bit speed selection (so, about 0.5% resolution)
* Also 1 stepper motors (unipolar or bipolar) with single coil, double coil, interleaved or micro-stepping.
* Up to 16 channels of PWM ouput from the fully-dedicated PWM driver chip which could be used for another 16 'hobby' servos or other usage. 
* Motors automatically disabled on power-up
* Big terminal block connectors to easily hook up wires (18-26AWG) and power without screwer
* Two LED indicators: RED LED indicate whether there is fault in motor driver and GREEN LED will light up when the board is properly powered up.
* Tested compatible with Arduino UNO, Leonardo, ADK/Mega R3, Due, Diecimila & Duemilanove.
* Download the easy-to-use Arduino software library, check out the examples and you're ready to go!

## MECHANICAL DETAILS

Details:

* Dimensions, assembled: 70mm x 55mm x 10mm 2.7"x2.1"x0.4"
* This board/chip uses I2C 7-bit addresses between 0x60-0x80, selectable with resistors array.

![](rohs_icon.jpg)
