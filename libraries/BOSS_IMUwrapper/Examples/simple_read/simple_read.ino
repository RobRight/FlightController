#include <mpu_input.h>
#include "Wire.h"
#include <EnableInterrupt.h>

#define IMU_INT 22

MPUinput IMU(IMU_INT);

// IMU interrupt wrapper
void IMPInterrupWrapper()
{
  IMU.imu_dmpDataReady();
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  
  enableInterrupt(22, IMPInterrupWrapper, RISING);
}

void loop()
{
  IMU.main_run();
  IMU.debug_serial();
}
