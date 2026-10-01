

#include "io.h"
#include "mcc_generated_files/system/pins.h"
#include "stdbool.h"

// take the supporting files form the Firmware_04 folder



// -------------------------- LED operations -------------------------------

//void GPIO_SetPowerLED(bool state) 
//{
//    LED_PWR_LAT = state ? 1 : 0;
//}

void GPIO_SetGreenLED(bool state) 
{
    Green_LED_LAT = state ? 1 : 0;
}

void GPIO_SetRedLED(bool state) 
{
    Red_LED_LAT = state ? 1 : 0;
}

//void GPIO_SetCalibrationLED(bool state) {
//    LED_CAL_LAT = state ? 1 : 0;
//}

//void GPIO_SetLogicOutput(bool state) 
//{
//    LOGIC_OUT_LAT = state ? 1 : 0;
//}
//
//bool GPIO_GetLogicOutput(void) 
//{
//    return (LOGIC_OUT_LAT == 1);
//}

void GPIO_ToggleRedLED(void) 
{
    Red_LED_LAT = Red_LED_LAT ? 0 : 1;
}

void GPIO_ToggleGreenLED(void) 
{
    Green_LED_LAT = Green_LED_LAT ? 0 : 1;
}







//========================================================================================================


// -------------------------- NPN-PNP operations -------------------------------








//========================================================================================================










