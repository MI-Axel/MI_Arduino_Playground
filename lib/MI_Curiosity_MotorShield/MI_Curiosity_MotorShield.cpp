/*!
 * @file Curiosity_MotorShield.cpp
 *
 * @mainpage Curiosity servo and DC motor driver
 *
 * @section intro_sec Introduction
 *
 * This is the library for the Curiosity Motor Shield V1 for Arduino. 
 * It supports DC motors & Servo motors. And it also provides the interface of I2C and SPI to sensors. 
 * This shield/wing uses I2C to communicate, 2 pins (SCL+SDA) are required
 * to interface.
 *
 * MuchInovation invests time and resources providing this open source code,
 * please support MuchInovation and open-source hardware by purchasing
 * products from MuchInovation!
 *
 * @section author Author
 *
 * Written by Axel from MuchInovation.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include "MI_Curiosity_MotorShield.h"


/**************************************************************************/
/*!
    @brief  Create the Motor Shield object at an I2C address, default is 0x60
    @param  addr Optional I2C address if you've changed it
*/
/**************************************************************************/
Curiosity_MotorShield::Curiosity_MotorShield(uint8_t addr) 
{
    uint8_t nSleepPin = 8;
    uint8_t Ain1 = 6;
    uint8_t Ain2 = 7;
    
    // avoid motor into parallel mode
    pinMode(nSleepPin, OUTPUT);
    pinMode(Ain1, OUTPUT);
    pinMode(Ain2, OUTPUT);

    digitalWrite(nSleepPin, LOW);
    digitalWrite(Ain1, LOW);
    digitalWrite(Ain2, LOW);
   
    _addr = addr;
    _pwm = Curiosity_MS_PWMServoDriver(_addr);
}


/**************************************************************************/
/*!
    @brief  Initialize the I2C hardware and PWM driver, then turn off all pins.
    @param    freq
    The PWM frequency for the driver, used for speed control and microstepping.
    By default we use 1600 Hz which is a little audible but efficient.
    @param    theWire
    A pointer to an optional I2C interface. If not provided, we use Wire or Wire1
    (on Due)
*/
/**************************************************************************/
void Curiosity_MotorShield::begin(uint16_t freq, TwoWire *theWire) {
  if (! theWire) {
#if defined(ARDUINO_SAM_DUE)
    _i2c = &Wire1;
#else
    _i2c = &Wire;
#endif
  } else {
    _i2c = theWire;
  }

  // init PWM w/_freq
  _i2c->begin();
  _pwm.begin();
  _freq = freq;
  _pwm.setPWMFreq(_freq);  // This is the maximum PWM frequency
  for (uint8_t i=0; i<16; i++) 
    _pwm.setPWM(i, 0, 0);
}


/**************************************************************************/
/*!
    @brief  Helper that sets the PWM output on a pin and manages 'all on or off'
    @param  pin The PWM output on the driver that we want to control (0-15)
    @param  value The 12-bit PWM value we want to set (0-4095) - 4096 is a special 'all on' value
*/
/**************************************************************************/
void Curiosity_MotorShield::setPWM(uint8_t pin, uint16_t value) {
  if (value > 4095) {
    _pwm.setPWM(pin, 4096, 0);
  } else 
    _pwm.setPWM(pin, 0, value);
}

/**************************************************************************/
/*!
    @brief  Helper that sets the PWM output on a pin as if it were a GPIO
    @param  pin The PWM output on the driver that we want to control (0-15)
    @param  value HIGH or LOW depending on the value you want!
*/
/**************************************************************************/
void Curiosity_MotorShield::setPin(uint8_t pin, boolean value) {
  if (value == LOW)
    _pwm.setPWM(pin, 0, 0);
  else
    _pwm.setPWM(pin, 4096, 0);
}


/**************************************************************************/
/*!
    @brief  Mini factory that will return a pointer to an already-allocated 
    Curiosity_DCMotor object. Initializes the DC motor and turns off all pins
    @param  num The DC motor port we want to use: 1 thru 2 are valid
    @returns NULL if something went wrong, or a pointer to a Curiosity_DCMotor
*/
/**************************************************************************/
Curiosity_DCMotor *Curiosity_MotorShield::getMotor(uint8_t num) 
{
    if (num > 2) return NULL;

    uint8_t nSleepPin = 8;

    num--;
//    
    digitalWrite(nSleepPin, HIGH);

    if (dcmotors[num].motornum == 0) 
    {
        // not init'd yet!
        dcmotors[num].motornum = num;
        dcmotors[num].MC = this;
        uint8_t pwm, dir;
        if (num == 0) 
        {
            pwm = 6; dir = 7;
        }
        else if (num == 1) 
        {
            pwm = 5; dir =4;
        }   
        dcmotors[num].PWMpin = pwm;
        dcmotors[num].DIRpin = dir;
        
        pinMode(pwm, OUTPUT);
        pinMode(dir, OUTPUT);
        // set motor release
        digitalWrite(pwm, LOW);
        digitalWrite(dir, LOW);
        dcmotors[num].motorStat = RELEASE;
    }
    return &dcmotors[num];
}


/******************************************
               MOTORS
******************************************/

/**************************************************************************/
/*!
    @brief  Create a DCMotor object, un-initialized!
    You should never call this, instead have the {@link Curiosity_MotorShield} 
    give you a DCMotor object with {@link Curiosity_MotorShield.getMotor}
*/
/**************************************************************************/
Curiosity_DCMotor::Curiosity_DCMotor(void) 
{
    MC = NULL;
    motornum = 0;
    PWMpin = DIRpin = 0;
    motorStat = 0;
}

/**************************************************************************/
/*!
    @brief  Control the DC Motor direction and action
    @param  cmd The action to perform, can be FORWARD, BACKWARD, RELEASE or BRAKE
    @param  spd The pwm value, 0 is off, 255 is fully on
*/
/**************************************************************************/
void Curiosity_DCMotor::run(uint8_t cmd, uint8_t spd) 
{
    switch (cmd) 
    {
        case FORWARD:
            digitalWrite(DIRpin, LOW);
            analogWrite(PWMpin, spd);
            motorStat = FORWARD;
            break;
        case BACKWARD:
            digitalWrite(DIRpin, HIGH);
            analogWrite(PWMpin, (255-spd));
            motorStat = BACKWARD;
            break;
        case RELEASE:
            digitalWrite(DIRpin, LOW);
            digitalWrite(PWMpin, LOW);
            motorStat = RELEASE;
            break;
        case BRAKE:
            digitalWrite(DIRpin, HIGH);
            digitalWrite(PWMpin, HIGH);
            motorStat = BRAKE;
            break;
        default:
            break;
    }
}

/**************************************************************************/
/*!
    @brief  Control the DC Motor speed/throttle
    @param  speed The 8-bit PWM value, 0 is off, 255 is on
*/
/**************************************************************************/
void Curiosity_DCMotor::setSpeed(uint8_t speed) 
{
    if(motorStat == FORWARD)
        analogWrite(PWMpin, speed);
    else if(motorStat == BACKWARD)
        analogWrite(PWMpin, (255-speed));
}


