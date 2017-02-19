/*
 * 
 * Written by: William Gregory
 * Name: ESC controller
 * Version: 1.0
 * Description: control the ESCs for 4 motors
 * INFO:
 * 
 * Motor Map: {FL, FR, BL, BR}
 *   FL FR
 *   BL BR
 * 
 * Control Input:
 *   send_motor_speeds(motorFL,motorFR,motorBL,motorBR)
 *   Value(0 to 100)
 * 
 * Motors run from:
 *  "writeMicroseconds" (1200 to 2000)
 *
 * Functions:
 * - ESCcontrol
 * - main_enable_esc
 * - main_run
 * - main_convert_values
 * - main_write_motors
 * - all_off
 * - debug_serial_test
 *
 */

 
// --INCLUDE--
#include "esc_control.h"
#include <Servo.h>


// --DEFINE--
#define esc_default_off 1000
#define esc_default_min 1300
#define esc_default_max 1900

//----------------------------------------------------
//----------------------------------------------------
// Variables
//----------------------------------------------------
//----------------------------------------------------

int esc_pin[4];
int motor_engage[4];

//-- State Variables:
// Control Input values: convert to output_val with 'convert_values();'
int input_val[4];
// ESC send values
int output_val[4];

// ESC(servo) initiate
Servo escFL, escFR, escBL, escBR;




//----------------------------------------------------
//----------------------------------------------------
// main functions
//----------------------------------------------------
//----------------------------------------------------


// Name: ESCcontrol
// Description: Initilize library
// Info:
//----------------------------------------------------
ESCcontrol::ESCcontrol(int pin1, int pin2, int pin3, int pin4, int eng1, int eng2, int eng3, int eng4)
{
  // assign pins
  esc_pin[0] = pin1;
  esc_pin[1] = pin2;
  esc_pin[2] = pin3;
  esc_pin[3] = pin4;
  
  // set motor engage
  motor_engage[0] = eng1;
  motor_engage[1] = eng2;
  motor_engage[2] = eng3;
  motor_engage[3] = eng4;
  
  // input/output values
  for(int x = 0; x <= 4; x++) input_val[x] = 0;
  for(int x = 0; x <= 4; x++) output_val[x] = 0;
}


// Name: main_enable_esc
// Description: enable ESCs
// Info: run once on startup
//----------------------------------------------------
void ESCcontrol::main_enable_esc()
{
  // attach ESCs to pins
  escFL.attach(esc_pin[0]);
  escFR.attach(esc_pin[1]);
  escBL.attach(esc_pin[2]);
  escBR.attach(esc_pin[3]);
  
  // default off
  escFL.writeMicroseconds(esc_default_off);
  escFR.writeMicroseconds(esc_default_off);
  escBL.writeMicroseconds(esc_default_off);
  escBR.writeMicroseconds(esc_default_off);
  delay(2000);
}


// Name: main_run
// Description: call for normal libarary operation
// Info:
//----------------------------------------------------
void ESCcontrol::main_run(int valFL, int valFR, int valBL, int valBR)
{
  // set input values
  input_val[0] = valFL;
  input_val[1] = valFR;
  input_val[2] = valBL;
  input_val[3] = valBR;

  // convert to esc values
  main_convert_values();

  // send values to ESCs
  main_write_motors(output_val[0], output_val[1], output_val[2], output_val[3]);
}


// Name: main_convert_values
// Description: convert input value to esc value
// Info:
//----------------------------------------------------
void ESCcontrol::main_convert_values()
{
  for(int x = 0; x < 4; x++) {
	if(input_val[x] == 0) output_val[x] = esc_default_off;
    else output_val[x] = map(input_val[x], 0, 100, esc_default_min, esc_default_max);
  }
}


// Name: main_write_motors
// Description: send motor values to ESCs
// Info:
//----------------------------------------------------
void ESCcontrol::main_write_motors(int valFL, int valFR, int valBL, int valBR)
{
  if (motor_engage[0] == 1) escFL.writeMicroseconds(valFL);
  if (motor_engage[1] == 1) escFR.writeMicroseconds(valFR);
  if (motor_engage[2] == 1) escBL.writeMicroseconds(valBL);
  if (motor_engage[3] == 1) escBR.writeMicroseconds(valBR);
}




//----------------------------------------------------
//----------------------------------------------------
// command functions
//----------------------------------------------------
//----------------------------------------------------


// Name: com_all_off
// Description: turn off all motors
// Info:
//----------------------------------------------------
void ESCcontrol::com_all_off()
{
  int val = esc_default_off;
  main_write_motors(val,val,val,val);
}




//----------------------------------------------------
//----------------------------------------------------
// debug functions
//----------------------------------------------------
//----------------------------------------------------


// Name: debug_serial_test
// Description: set motor values to input serial value
// Info: debug ESCcontrol libarary
//----------------------------------------------------
void ESCcontrol::debug_serial_test()
{
  if(Serial.available() > 0)
  {
    int val = Serial.parseInt();
    main_write_motors(val, val, val, val);
    Serial.println(val);
  }
}