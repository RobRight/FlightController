// GPS test

#include <gps_wrapper.h>

// Initiate GPS Library
GPSwrapper GPS;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  GPS.main_run();
}