#ifndef GPS_WRAPPER
#define GPS_WRAPPER

#include "arduino.h"

class GPSwrapper
{
  private:
    void get_values();
  public:
    GPSwrapper();
	void main_run();
	void target_info(double target_lat, double target_lng);
	void debug_display_info();
	bool gps_pos_valid, gps_date_valid, gps_time_valid;
	double gps_pos_latitude,gps_pos_longitude;
	double gps_pos_altitude, gps_pos_speed, gps_pos_course;
	double gps_date_year, gps_date_month, gps_date_day;
	double gps_time_hour, gps_time_minute, gps_time_second, gps_time_centisecond;
	long gps_hdop_value;
};

#endif
