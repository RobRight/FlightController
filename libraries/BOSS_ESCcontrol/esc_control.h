#ifndef ESC_control
#define ESC_control

#include "arduino.h"

class ESCcontrol
{
  private:
    void main_convert_values();
    int input_val[4], output_val[4];
	int esc_pin[4], motor_engage[4];
  public:
    ESCcontrol(int pin1, int pin2, int pin3, int pin4, int eng1, int eng2, int eng3, int eng4);
	void main_write_motors(int valFL, int valFR, int valBL, int valBR);
    void main_run(int valFL, int valFR, int valBL, int valBR);
	void com_all_off();
	void main_enable_esc();
	void debug_serial_test();
	
};

#endif
