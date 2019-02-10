/******************************************************************
 This is the library for the Curiosity Motor Shield V2 for Arduino. 
 It supports DC motors & Stepper motors with microstepping as well
 as stacking-support. It is *not* compatible with the V1 library!

 It will only work with https://www.adafruit.com/products/1483
 
 Curiosity invests time and resources providing this open
 source code, please support Curiosity and open-source hardware
 by purchasing products from Curiosity!
 
 Written by Limor Fried/Ladyada for Curiosity Industries.
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.
 ******************************************************************/

#ifndef _Curiosity_MotorShield_h_
#define _Curiosity_MotorShield_h_

#include <Arduino.h>
#include <inttypes.h>
#include <Wire.h>
#include "utility/MI_Curiosity_MS_PWMServoDriver.h"

//#define MOTORDEBUG
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4




class Curiosity_MotorShield;

/** Object that controls and keeps state for a single DC motor */
class Curiosity_DCMotor
{
 public:
  Curiosity_DCMotor(void);
  friend class Curiosity_MotorShield;  ///< Let MotorShield create DCMotors
  void run(uint8_t cmd, uint8_t spd);
  void setSpeed(uint8_t speed);
  
 private:
  uint8_t PWMpin, DIRpin;
  Curiosity_MotorShield *MC;
  uint8_t motornum;
  uint8_t motorStat;                // motor has four possible status: FORWARD, BACKWARD, BRAKE, RELEASE
};


/** Object that controls and keeps state for the whole motor shield. 
    Use it to create DC and Stepper motor objects! */
class Curiosity_MotorShield
{
  public:
    Curiosity_MotorShield(uint8_t addr = 0x70);

    void begin(uint16_t freq = 1600, TwoWire *theWire = NULL);
    Curiosity_DCMotor *getMotor(uint8_t n);

    friend class Curiosity_DCMotor;   ///< Let DCMotors control the Shield

    void setPWM(uint8_t pin, uint16_t val);
    void setPin(uint8_t pin, boolean val);
 private:
    TwoWire *_i2c;
    uint8_t _addr;
    uint16_t _freq;
    Curiosity_DCMotor dcmotors[2];
    Curiosity_MS_PWMServoDriver _pwm;
};

#endif
