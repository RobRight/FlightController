/* 
 * 
 * William Gregory
 * 
 * 
 */

#include <esc_control.h>

// ESC PINS
#define ESC_FL 4
#define ESC_FR 5
#define ESC_BL 6
#define ESC_BR 7

// Initalize ESC Libarary
ESCcontrol ESC(ESC_FL, ESC_FR, ESC_BL, ESC_BR);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  ESC.main_run(0,0,0,0);
}
