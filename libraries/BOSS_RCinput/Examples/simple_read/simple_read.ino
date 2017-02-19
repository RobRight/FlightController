/*
 *
 * William Gregory
 * RC Reciever Input Library
 * December 2015
 * 
 */

#include <rc_input.h>
#include <EnableInterrupt.h>

// RC PINS
#define RC_CH1 9
#define RC_CH2 10
#define RC_CH3 11
#define RC_CH4 12
// RC MODE
// transmitter input mode (map values)
// 1: (roll, throttle, pitch, yaw)  2: (roll, pitch, throttle, yaw)  3: (yaw, throttle, pitch, roll)  4: (yaw, pitch, throttle, roll)
#define RC_MODE 1

RCinput RC(RC_CH1, RC_CH2, RC_CH3, RC_CH4, RC_MODE);

// Interrupt wrapper
void RCInterrupWrapper()
{
  RC.main_read();
}

void setup()
{
  
  // Serial
  Serial.begin(9600);
  
  // enable reciever interrupts
  enableInterrupt(RC.reciever_pins[0], RCInterrupWrapper, CHANGE);
  enableInterrupt(RC.reciever_pins[1], RCInterrupWrapper, CHANGE);
  enableInterrupt(RC.reciever_pins[2], RCInterrupWrapper, CHANGE);
  enableInterrupt(RC.reciever_pins[3], RCInterrupWrapper, CHANGE);
}

void loop()
{
  RC.debug_serial();
  delay(1000);
}
