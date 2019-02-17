#include <Arduino.h>
#include <Wire.h>
#include <Servo.h> 
#include "MI_Curiosity_MotorShield.h"


// Macro definition for servo
#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50
// We'll also test out the built in Arduino Servo library
//Servo servo1;
// Create the motor shield object with the default I2C address
Curiosity_MotorShield MI_CMS = Curiosity_MotorShield(0x60);
// Or, create it with a different I2C address (say for stacking)
// Remember to add more resistors if you want to stack one more board!

// And connect a DC motor to port MA and MB
Curiosity_DCMotor *DC_Motor_A = MI_CMS.getMotor(1);

Curiosity_DCMotor *DC_Motor_B = MI_CMS.getMotor(2);


void setup() 
{
	Serial.begin(115200);
    
    Serial.println("MMMMotor party!");
    MI_CMS.begin(FREQUENCY);  // create with the frequency 50Hz
    //MI_CMS.begin(1000);  // OR with a different frequency, say 1KHz
  
    // Attach a servo to pin #10
//    servo1.attach(10);
   
    // turn on motor M1
    DC_Motor_A->run(RELEASE, 0);
    DC_Motor_B->run(RELEASE, 0);

    // you could set any channel with any dutycycle you like!
    MI_CMS.setPWM(1, 3584);
    MI_CMS.setPWM(3, 2560);
    MI_CMS.setPWM(5, 1536);
    MI_CMS.setPWM(7, 512);

    // And you could also make these PWM output channel work as GPIO.
    MI_CMS.setPin(10, LOW);
    MI_CMS.setPin(12, HIGH);
    MI_CMS.setPin(14, LOW);

    // We use channels 8 to run hobby servo motor as an example.
    MI_CMS.setPWM(8, 0);
}

// this function is used to convert the servo motor angle to PWM dutycycle value
int pulseWidth(int angle)
{
    int pulse_wide, analog_value;
    pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
    Serial.println(analog_value);
    return analog_value;
}

int i;
void loop() 
{
// we run motor A forward first
    DC_Motor_B->run(FORWARD, 0);
    DC_Motor_A->run(FORWARD, 80);

// And set the servo motor of channel 8 keeps on 0 degree position    
    MI_CMS.setPWM(8, pulseWidth(0));

// And motor B starts to accelerate    
    for (i=0; i<255; i++) 
    {
//        servo1.write(map(i, 0, 255, 0, 180));
        DC_Motor_B->setSpeed(i);
        delay(3);
    }
 
// We change the servo motor position from 0 degree to 120 degree    
    MI_CMS.setPWM(8, pulseWidth(120));

// We reduce the speed of motor B and Brake motor A    
    for (i=255; i!=0; i--) 
    {
//        servo1.write(map(i, 0, 255, 0, 180));
        DC_Motor_B->setSpeed(i);
        DC_Motor_A->run(BRAKE, 0);          // when you brake motor, the spd value is not important.
        delay(3);
    }
 
    DC_Motor_B->run(BACKWARD, 0);
    DC_Motor_A->run(BACKWARD, 80);
    
// We change the servo motor position from 120 degree to 180 degree    
    MI_CMS.setPWM(8, pulseWidth(180));

    for (i=0; i<255; i++) 
    {
//        servo1.write(map(i, 0, 255, 0, 180));
        DC_Motor_B->setSpeed(i);  
        delay(3);
    }
 
    MI_CMS.setPWM(8, pulseWidth(90));
    for (i=255; i!=0; i--) 
    {
//        servo1.write(map(i, 0, 255, 0, 180));
        DC_Motor_B->setSpeed(i);
        DC_Motor_A->run(RELEASE, 0);          // when you release motor, the spd value is not important either.
        delay(3);
    }

    Serial.println("One loop is finished!");
}
