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
Curiosity_MotorShield MI_CMS = Curiosity_MotorShield(0x70);
// Or, create it with a different I2C address (say for stacking)
// Curiosity_MotorShield MI_CMS = Curiosity_MotorShield(0x60); 

// And connect a DC motor to port M1
Curiosity_DCMotor *DC_Motor_A = MI_CMS.getMotor(1);

Curiosity_DCMotor *DC_Motor_B = MI_CMS.getMotor(2);


void setup() 
{
	Serial.begin(115200);
    
    Serial.println("MMMMotor party!");
    MI_CMS.begin(60);  // create with the frequency 60Hz
    //MI_CMS.begin(1000);  // OR with a different frequency, say 1KHz
  
    // Attach a servo to pin #10
//    servo1.attach(10);
   
    // turn on motor M1
    DC_Motor_A->run(RELEASE, 0);
    DC_Motor_B->run(RELEASE, 0);

    MI_CMS.setPWM(0, 4096);
    MI_CMS.setPWM(1, 3584);
    MI_CMS.setPWM(2, 3072);
    MI_CMS.setPWM(3, 2560);
    MI_CMS.setPWM(4, 2048);
    MI_CMS.setPWM(5, 1536);
    MI_CMS.setPWM(6, 1024);
    MI_CMS.setPWM(7, 512);
    MI_CMS.setPWM(8, 0);
}

// use for calculate servo angle
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
    DC_Motor_B->run(FORWARD, 0);

    MI_CMS.setPWM(8, pulseWidth(0));
    for (i=0; i<255; i++) 
    {
//        servo1.write(map(i, 0, 255, 0, 180));
        DC_Motor_B->setSpeed(i);
        delay(3);
    }
 
    MI_CMS.setPWM(8, pulseWidth(120));
    
    for (i=255; i!=0; i--) 
    {
//        servo1.write(map(i, 0, 255, 0, 180));
        DC_Motor_B->setSpeed(i);  
        delay(3);
    }
 
    DC_Motor_B->run(BACKWARD, 0);

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
        delay(3);
    }

    Serial.println("One loop is finished!");
}
