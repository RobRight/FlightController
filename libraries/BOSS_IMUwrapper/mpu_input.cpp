/*
 * 
 * Library format by: William Gregory
 * Originally by: 
 * Description: IMU (MPU6050) Input Library
 * Version: 1.2
 * December 2015
 *
 * ADD: euler
 * 
 * Functions:
 *   main_run
 *   debug_serial
 *
 */
 
// GPS test

#include "mpu_input.h"

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

// custom interupt pin
int interuptPin;

// MPU control/status vars
bool imu_dmpReady = false;  // set true if DMP init was successful
uint8_t imu_mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t imu_devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t imu_packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t imu_fifoCount;     // count of all bytes currently in FIFO
uint8_t imu_fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion imu_q;           // [w, x, y, z]         quaternion container
VectorFloat imu_gravity;    // [x, y, z]            gravity vector
float imu_ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

void MPUinput::imu_dmpDataReady() { mpuInterrupt = true; }

// Initialize
//----------------------------------------------------

/* ============================================
  I2Cdev device library code is placed under the MIT license
  Copyright (c) 2012 Jeff Rowberg
  https://github.com/sparkfun/MPU-9150_Breakout/tree/master/firmware

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  ===============================================
*/

MPUinput::MPUinput(int INTpin)
{
  interuptPin = INTpin;
	
  // initialize device
  mpu.initialize();

  // verify connection
  int test_con_val = mpu.testConnection();
  if (test_con_val != 1) Serial.println("MPU6050 connection failed");

  // load and configure the DMP
  imu_devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (imu_devStatus == 0) {
    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    imu_mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    imu_dmpReady = true;

    // get expected DMP packet size for later comparison
    imu_packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("IMU: DMP Initialization failed (code "));
    Serial.print(imu_devStatus);
    Serial.println(F(")"));
  }
}


void MPUinput::main_run() {
  // if programming failed, don't try to do anything
  if (!imu_dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && imu_fifoCount < imu_packetSize) {}

  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  imu_mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  imu_fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((imu_mpuIntStatus & 0x10) || imu_fifoCount == 1024)
  {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println(F("IMU: FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  }
  else if (imu_mpuIntStatus & 0x02)
  {
    // wait for correct available data length, should be a VERY short wait
    while (imu_fifoCount < imu_packetSize) imu_fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(imu_fifoBuffer, imu_packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    imu_fifoCount -= imu_packetSize;

    // get Yaw Pitch Roll
    mpu.dmpGetQuaternion(&imu_q, imu_fifoBuffer);
    mpu.dmpGetGravity(&imu_gravity, &imu_q);
    mpu.dmpGetYawPitchRoll(imu_ypr, &imu_q, &imu_gravity);
	
	// convert YPR to degrees
	imu_ypr[0] = imu_ypr[0] * 180 / M_PI;
	imu_ypr[1] = imu_ypr[1] * 180 / M_PI;
	imu_ypr[2] = imu_ypr[2] * 180 / M_PI;
  }
}

void MPUinput::debug_serial()
{
  Serial.print("ypr\t");
  Serial.print(imu_ypr[0]);
  Serial.print("\t");
  Serial.print(imu_ypr[1]);
  Serial.print("\t");
  Serial.println(imu_ypr[2]);
}