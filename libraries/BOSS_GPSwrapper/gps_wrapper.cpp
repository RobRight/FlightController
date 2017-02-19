/*
 * 
 * Written by: William Gregory
 * Description: RC Reciever Input Library
 * Version: 1.1
 * December 2015
 *
 * ADD: map function
 * 
 * Array format: {ch1, ch2, ch3, ch4}
 * 
 * Functions:
 *   main_run
 *   read_values
 *   debug_print_values: print read values to serial
 *
 */
 
// GPS test

#include "gps_wrapper.h"

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define GPS_PIN 3

// -- State Variables
// gps valid
bool gps_pos_valid;
bool gps_date_valid;
bool gps_time_valid;
// gps position
double gps_pos_latitude;
double gps_pos_longitude;
double gps_pos_altitude; // meters
double gps_pos_speed; // mph
double gps_pos_course; // degrees
// gps date
double gps_date_year;
double gps_date_month;
double gps_date_day;
// gps time
double gps_time_hour;
double gps_time_minute;
double gps_time_second;
double gps_time_centisecond;
// other
long gps_hdop_value;

// The serial connection to the GPS device
SoftwareSerial GPSserial(GPS_PIN, 0);

// The TinyGPS++ object
TinyGPSPlus TinyGPSPlus;

// Initialize
//----------------------------------------------------

GPSwrapper::GPSwrapper()
{
  GPSserial.begin(9600); // GPS serial baud
  bool gps_pos_valid = false;
  bool gps_date_valid = false;
  bool gps_time_valid = false;
}

// main function
void GPSwrapper::main_run()
{
  while (GPSserial.available() > 0)
    if (TinyGPSPlus.encode(GPSserial.read()))
      get_values();
  
  if (millis() > 5000 && TinyGPSPlus.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
  }
  
  get_values();
  //debug_display_info();
}

// get values from gps
void GPSwrapper::get_values()
{
  // position
  if (TinyGPSPlus.location.isValid())
  {
	gps_pos_valid = true;
    gps_pos_latitude = TinyGPSPlus.location.lat();
    gps_pos_longitude = TinyGPSPlus.location.lng();
  }
  else gps_pos_valid = false;
  // altitude
  if (TinyGPSPlus.altitude.isValid())
  {
	gps_pos_altitude = TinyGPSPlus.altitude.meters();
  }
  // date
  if (TinyGPSPlus.date.isValid())
  {
	gps_date_valid = true;
	gps_date_year = TinyGPSPlus.date.year();
	gps_date_month = TinyGPSPlus.date.month();
	gps_date_day = TinyGPSPlus.date.day();
  }
  else gps_date_valid = false;
  // time
  if (TinyGPSPlus.time.isValid())
  {
	gps_time_valid = true;
	gps_time_hour = TinyGPSPlus.time.hour();
	gps_time_minute = TinyGPSPlus.time.minute();
	gps_time_second = TinyGPSPlus.time.second();
	gps_time_centisecond = TinyGPSPlus.time.centisecond();
  }
  else gps_time_valid = false;
  
  gps_pos_speed = TinyGPSPlus.speed.mph(); // miles per hour
  gps_pos_course = TinyGPSPlus.course.deg(); // course in degrees
  gps_hdop_value = TinyGPSPlus.hdop.value(); // Horizontal Dim. of Precision (i32)
}

// return distance and direction to given target
void target_info(double target_lat, double target_lng)
{
  // distance to target in kilo meters
  double distance_to_target = TinyGPSPlus.distanceBetween(gps_pos_latitude, gps_pos_longitude, target_lat, target_lng) / 1000.0;
  double course_to_target = TinyGPSPlus.courseTo(gps_pos_latitude, gps_pos_longitude, target_lat, target_lng);
}

// print gps info to serial
void GPSwrapper::debug_display_info()
{
  if(gps_pos_valid)
  {  
    Serial.print("gps_pos_latitude: "); Serial.println(gps_pos_latitude);
    Serial.print("gps_pos_longitude: "); Serial.println(gps_pos_longitude);
	Serial.print("gps_pos_altitude: "); Serial.println(gps_pos_altitude);
	Serial.print("gps_pos_speed: "); Serial.println(gps_pos_speed);
	Serial.print("gps_pos_course: "); Serial.println(gps_pos_course);
  }
  if(gps_date_valid)
  {
	Serial.print("GPS date: (month/day/year)");
	Serial.println(gps_date_month); Serial.print("/");
	Serial.print(gps_date_day); Serial.print("/");
	Serial.println(gps_date_year);
  }
  if(gps_time_valid)
  {
	Serial.print("GPS time: (hour:minute:second)");
	Serial.println(gps_time_hour); Serial.print(":");
	Serial.print(gps_time_minute); Serial.print(":");
	Serial.println(gps_time_second);
  }
}
