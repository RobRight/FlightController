#ifndef RC_INPUT
#define RC_INPUT

#include "arduino.h"

class RCinput
{
  private:
    void main_value_map();
	void main_channel_assign();
    volatile int read_values[4];
    volatile unsigned long read_period[4];
	int default_values_min[4], default_values_max[4];
  public:
    RCinput(int ch1, int ch2, int ch3, int ch4, int mode);
    void interrupt_handler(int val);
	void main_range_calibrate();
	void main_run();
    void debug_serial();
	int error;
	int reciever_pins[4];
	int controller_mode;
	int map_throttle, map_yaw, map_pitch, map_roll;
};

#endif
