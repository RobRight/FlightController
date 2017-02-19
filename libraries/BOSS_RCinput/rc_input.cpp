/*
 * 
 * Written by: William Gregory
 * Description: RC Reciever Input Managment Library
 * Version: 1.3
 * Date: December 2015 - Present
 * 
 * Array format: {ch1, ch2, ch3, ch4}
 * 
 * Functions:
 *	 - RCinput: library initilize
 *   - interrupt_handler: interrupt called function
 *   - main_run: call for normal library usage
 *   - main_value_map: map values
 *   - main_channel_assign: map chanells
 *   - main_range_calibrate: calibrate min/max values
 *   - debug_serial: print rc values to serial
 *
 */
 
 
 
 
// --INCLUDE--
// Include library
#include "rc_input.h"


// --DEFINE--
// default rc min/max input values
// use calibrate to get actual values
#define default_input_min 1200
#define default_input_max 1800




//----------------------------------------------------
//----------------------------------------------------
// Variables
//----------------------------------------------------
//----------------------------------------------------


//-- General Variables
// rc pins
int reciever_pins[4];

// controller input value range
int default_values_min[4];
int default_values_max[4];

//-- State Variables
// true if there is an error (usually form user config)
int error;

// read reciever values
volatile int read_values[4];
// rc interupt read period
volatile unsigned long read_period[4];

// transmitter input mode (map values)
// 1: (roll, throttle, pitch, yaw)  2: (roll, pitch, throttle, yaw)  3: (yaw, throttle, pitch, roll)  4: (yaw, pitch, throttle, roll)
int controller_mode;

// mapped values
int map_values[4];

// assigned values
int map_throttle;
int map_yaw;
int map_pitch;
int map_roll;




//----------------------------------------------------
//----------------------------------------------------
// main functions
//----------------------------------------------------
//----------------------------------------------------


// Name: RCinput
// Description: Initilize library
// Info: 
//----------------------------------------------------
RCinput::RCinput(int ch1, int ch2, int ch3, int ch4, int mode)
{
  // default values
  error = 0;
  // assign pins
  reciever_pins[0] = ch1;
  reciever_pins[1] = ch2;
  reciever_pins[2] = ch3;
  reciever_pins[3] = ch4;
  // set mode
  controller_mode = mode;
  // default values
  for (int x=0; x<4; x++)
  {
	read_values[x] = 0;
	read_period[x] = 0;
	map_values[x] = 0;
	default_values_min[x] = default_input_min;
	default_values_max[x] = default_input_max;
  }
  // default values
  map_throttle = 0;
  map_yaw = 0;
  map_pitch = 0;
  map_roll = 0;
  
  
}


// Name: interrupt_handler
// Description: interrupt rc read function
// Info: triggered with interrupt
//----------------------------------------------------
void RCinput::interrupt_handler(int val)
{
  // check if time is 0 ( true: time=now; false: value=(now-time); )
  if(read_period[val] == 0) read_period[val] = micros();
  else
  {
    read_values[val] = (int)(micros() - read_period[val]);
    read_period[val] = 0;
  }
}



// Name: main_run
// Description: runs library functions
// Info: call for normal library usage
//----------------------------------------------------
void RCinput::main_run()
{
  main_value_map();
  main_channel_assign();
}


// Name: main_map
// Description: map raw RC input values
// Info: throttle output: (-50 to 50); else output: (0 to 100)
//----------------------------------------------------
void RCinput::main_value_map()
{
  // throttle (0 to 100)
  map_values[0] = map(read_values[0], default_values_min[0], default_values_max[0], 0, 100);
  if(map_values[0] > 100) map_values[0] = 100;
  else if(map_values[0] < 100) map_values[0] = 0;
  // other (-50 to 50)
  for (int x=1; x<4; x++)
  {
	map_values[x] = map(read_values[x], default_values_min[x], default_values_max[x], -50, 50);
	if(map_values[x] > 50) map_values[x] = 100;
    else if(map_values[x] < -50) map_values[x] = 0;
  }
}


// Name: main_assign
// Description: map input values to the correct variable based on controller mode
// Info:
// 1: (roll, throttle, pitch, yaw)
// 2: (roll, pitch, throttle, yaw)
// 3: (yaw, throttle, pitch, roll)
// 4: (yaw, pitch, throttle, roll)
//----------------------------------------------------
void RCinput::main_channel_assign()
{
  // map values to correct input
  if(controller_mode == 1)
  {	
	map_throttle = map_values[1];
	map_yaw = map_values[3];
	map_pitch = map_values[2];
	map_roll = map_values[0];
  }
  else if(controller_mode == 1)
  {	
	map_throttle = map_values[2];
	map_yaw = map_values[3];
	map_pitch = map_values[1];
	map_roll = map_values[0];
  }  
  else if(controller_mode == 1)
  {	
	map_throttle = map_values[1];
	map_yaw = map_values[0];
	map_pitch = map_values[2];
	map_roll = map_values[3];
  }  
  else if(controller_mode == 1)
  {	
	map_throttle = map_values[2];
	map_yaw = map_values[0];
	map_pitch = map_values[1];
	map_roll = map_values[3];
  }
  // unknown input configuration
  // throw error# 202
  else error = 202;
}


// Name: main_calibrate
// Description: Calibrate RC input min/max
// Info: Run once at start
//----------------------------------------------------
void RCinput::main_range_calibrate()
{
  unsigned long startTime = millis();
  unsigned long timePassed = 0;
  
  while(timePassed < 10000)
  {
	for (int x = 0; x <= 4; x++)
	{
	if(read_values[x] > default_values_min[x]) default_values_min[x] = read_values[x];
	if(read_values[x] > default_values_max[x]) default_values_max[x] = read_values[x];
    }
	
	timePassed = startTime - millis();
  }
}




//----------------------------------------------------
//----------------------------------------------------
// debug functions
//----------------------------------------------------
//----------------------------------------------------


// Name: debug_serial
// Description: print reciever values to Serial
// Info: must also call function 'main_run'
//----------------------------------------------------
void RCinput::debug_serial()
{
  // print
  Serial.println("");
  // read
  Serial.println("read values");
  Serial.print("CH 1_read:"); Serial.println(read_values[0]);
  Serial.print("CH 2_read:"); Serial.println(read_values[1]);
  Serial.print("CH 3_read:"); Serial.println(read_values[2]);
  Serial.print("CH 4_read:"); Serial.println(read_values[3]);
  // mapped
  Serial.println("mapped values");
  Serial.print("CH 1_map:"); Serial.println(map_values[0]);
  Serial.print("CH 2_map:"); Serial.println(map_values[1]);
  Serial.print("CH 3_map:"); Serial.println(map_values[2]);
  Serial.print("CH 4_map:"); Serial.println(map_values[3]);
}
