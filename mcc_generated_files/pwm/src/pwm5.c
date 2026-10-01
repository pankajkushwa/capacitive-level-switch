/**
  * PWM5 Generated Driver File
  *
  * @file pwm5.c
  *
  * @ingroup pwm5
  *
  * @brief This file contains the API implementations for the PWM5 module.
  *
  * @version PWM5 Driver Version 2.0.5
*/

 /*
© [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
 
 /**
  * Section: Included Files
  */

 #include <xc.h>
 #include "../pwm5.h"

 /**
  * Section: PWM Module APIs
  */

 void PWM5_Initialize(void)
 {
    // Set the PWM5 to the options selected in the User Interface
    
    // PWMPOL active_hi; PWMEN enabled; 
    PWM5CON = (uint8_t)0x80;
    
    // PWMDCH 127; 
    PWM5DCH = (uint8_t)0x7F;

    // PWMDCL 3; 
    PWM5DCL = (uint8_t)0xC0;
    
    CCPTMRS1bits.P5TSEL = (uint8_t)0x1;


 }

 void PWM5_LoadDutyValue(uint16_t dutyValue)
 {
     // Writing to 8 MSBs of PWM duty cycle in PWMDCH register
     PWM5DCH = (uint8_t) ((dutyValue & 0x03FCu) >> 2);
     
     // Writing to 2 LSBs of PWM duty cycle in PWMDCL register
     PWM5DCL = (uint8_t) ((dutyValue & 0x0003u) << 6);
 }
