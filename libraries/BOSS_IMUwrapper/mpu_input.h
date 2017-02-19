#ifndef MPU_INPUT
#define MPU_INPUT

#include "arduino.h"
#include "helper_3dmath.h"

class MPUinput
{
  private:
    bool imu_dmpReady;
	uint8_t imu_mpuIntStatus, imu_devStatus;
	uint16_t imu_packetSize, imu_fifoCount;
	uint8_t imu_fifoBuffer[64];
    Quaternion imu_q;
    VectorFloat imu_gravity;
	volatile bool mpuInterrupt;
  public:
    MPUinput(int INTpin);
	void main_run();
	void debug_serial();
	void imu_dmpDataReady();
	float imu_ypr[3];
	int interuptPin;
};

#endif
