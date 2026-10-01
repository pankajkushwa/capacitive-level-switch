/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.1.1
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

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA4 aliases
#define IO_RA4_TRIS                 TRISAbits.TRISA4
#define IO_RA4_LAT                  LATAbits.LATA4
#define IO_RA4_PORT                 PORTAbits.RA4
#define IO_RA4_WPU                  WPUAbits.WPUA4
#define IO_RA4_OD                   ODCONAbits.ODCA4
#define IO_RA4_ANS                  ANSELAbits.ANSELA4
#define IO_RA4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define IO_RA4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define IO_RA4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define IO_RA4_GetValue()           PORTAbits.RA4
#define IO_RA4_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define IO_RA4_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define IO_RA4_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define IO_RA4_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define IO_RA4_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define IO_RA4_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define IO_RA4_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define IO_RA4_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set RB1 aliases
#define Green_LED_TRIS                 TRISBbits.TRISB1
#define Green_LED_LAT                  LATBbits.LATB1
#define Green_LED_PORT                 PORTBbits.RB1
#define Green_LED_WPU                  WPUBbits.WPUB1
#define Green_LED_OD                   ODCONBbits.ODCB1
#define Green_LED_ANS                  ANSELBbits.ANSELB1
#define Green_LED_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define Green_LED_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define Green_LED_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define Green_LED_GetValue()           PORTBbits.RB1
#define Green_LED_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define Green_LED_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define Green_LED_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define Green_LED_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define Green_LED_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define Green_LED_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define Green_LED_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define Green_LED_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set RB2 aliases
#define Rotary_SW1_TRIS                 TRISBbits.TRISB2
#define Rotary_SW1_LAT                  LATBbits.LATB2
#define Rotary_SW1_PORT                 PORTBbits.RB2
#define Rotary_SW1_WPU                  WPUBbits.WPUB2
#define Rotary_SW1_OD                   ODCONBbits.ODCB2
#define Rotary_SW1_ANS                  ANSELBbits.ANSELB2
#define Rotary_SW1_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define Rotary_SW1_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define Rotary_SW1_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define Rotary_SW1_GetValue()           PORTBbits.RB2
#define Rotary_SW1_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define Rotary_SW1_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define Rotary_SW1_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define Rotary_SW1_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define Rotary_SW1_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define Rotary_SW1_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define Rotary_SW1_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define Rotary_SW1_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set RB3 aliases
#define Rotary_SW2_TRIS                 TRISBbits.TRISB3
#define Rotary_SW2_LAT                  LATBbits.LATB3
#define Rotary_SW2_PORT                 PORTBbits.RB3
#define Rotary_SW2_WPU                  WPUBbits.WPUB3
#define Rotary_SW2_OD                   ODCONBbits.ODCB3
#define Rotary_SW2_ANS                  ANSELBbits.ANSELB3
#define Rotary_SW2_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define Rotary_SW2_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define Rotary_SW2_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define Rotary_SW2_GetValue()           PORTBbits.RB3
#define Rotary_SW2_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define Rotary_SW2_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define Rotary_SW2_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define Rotary_SW2_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define Rotary_SW2_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define Rotary_SW2_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define Rotary_SW2_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define Rotary_SW2_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set RB4 aliases
#define Rotary_SW4_TRIS                 TRISBbits.TRISB4
#define Rotary_SW4_LAT                  LATBbits.LATB4
#define Rotary_SW4_PORT                 PORTBbits.RB4
#define Rotary_SW4_WPU                  WPUBbits.WPUB4
#define Rotary_SW4_OD                   ODCONBbits.ODCB4
#define Rotary_SW4_ANS                  ANSELBbits.ANSELB4
#define Rotary_SW4_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define Rotary_SW4_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define Rotary_SW4_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define Rotary_SW4_GetValue()           PORTBbits.RB4
#define Rotary_SW4_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define Rotary_SW4_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define Rotary_SW4_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define Rotary_SW4_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define Rotary_SW4_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define Rotary_SW4_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define Rotary_SW4_SetAnalogMode()      do { ANSELBbits.ANSELB4 = 1; } while(0)
#define Rotary_SW4_SetDigitalMode()     do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set RB5 aliases
#define Rotary_SW8_TRIS                 TRISBbits.TRISB5
#define Rotary_SW8_LAT                  LATBbits.LATB5
#define Rotary_SW8_PORT                 PORTBbits.RB5
#define Rotary_SW8_WPU                  WPUBbits.WPUB5
#define Rotary_SW8_OD                   ODCONBbits.ODCB5
#define Rotary_SW8_ANS                  ANSELBbits.ANSELB5
#define Rotary_SW8_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define Rotary_SW8_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define Rotary_SW8_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define Rotary_SW8_GetValue()           PORTBbits.RB5
#define Rotary_SW8_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define Rotary_SW8_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define Rotary_SW8_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define Rotary_SW8_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define Rotary_SW8_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define Rotary_SW8_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define Rotary_SW8_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define Rotary_SW8_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set RC2 aliases
#define IO_RC2_TRIS                 TRISCbits.TRISC2
#define IO_RC2_LAT                  LATCbits.LATC2
#define IO_RC2_PORT                 PORTCbits.RC2
#define IO_RC2_WPU                  WPUCbits.WPUC2
#define IO_RC2_OD                   ODCONCbits.ODCC2
#define IO_RC2_ANS                  ANSELCbits.ANSELC2
#define IO_RC2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define IO_RC2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define IO_RC2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define IO_RC2_GetValue()           PORTCbits.RC2
#define IO_RC2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define IO_RC2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define IO_RC2_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define IO_RC2_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define IO_RC2_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define IO_RC2_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define IO_RC2_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define IO_RC2_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set RC5 aliases
#define Red_LED_TRIS                 TRISCbits.TRISC5
#define Red_LED_LAT                  LATCbits.LATC5
#define Red_LED_PORT                 PORTCbits.RC5
#define Red_LED_WPU                  WPUCbits.WPUC5
#define Red_LED_OD                   ODCONCbits.ODCC5
#define Red_LED_ANS                  ANSELCbits.ANSELC5
#define Red_LED_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define Red_LED_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define Red_LED_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define Red_LED_GetValue()           PORTCbits.RC5
#define Red_LED_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define Red_LED_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define Red_LED_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define Red_LED_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define Red_LED_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define Red_LED_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define Red_LED_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define Red_LED_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set RC6 aliases
#define IO_RC6_TRIS                 TRISCbits.TRISC6
#define IO_RC6_LAT                  LATCbits.LATC6
#define IO_RC6_PORT                 PORTCbits.RC6
#define IO_RC6_WPU                  WPUCbits.WPUC6
#define IO_RC6_OD                   ODCONCbits.ODCC6
#define IO_RC6_ANS                  ANSELCbits.ANSELC6
#define IO_RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define IO_RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define IO_RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define IO_RC6_GetValue()           PORTCbits.RC6
#define IO_RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define IO_RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define IO_RC6_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define IO_RC6_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define IO_RC6_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define IO_RC6_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define IO_RC6_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define IO_RC6_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 aliases
#define IO_RC7_TRIS                 TRISCbits.TRISC7
#define IO_RC7_LAT                  LATCbits.LATC7
#define IO_RC7_PORT                 PORTCbits.RC7
#define IO_RC7_WPU                  WPUCbits.WPUC7
#define IO_RC7_OD                   ODCONCbits.ODCC7
#define IO_RC7_ANS                  ANSELCbits.ANSELC7
#define IO_RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define IO_RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define IO_RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define IO_RC7_GetValue()           PORTCbits.RC7
#define IO_RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define IO_RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define IO_RC7_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define IO_RC7_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define IO_RC7_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define IO_RC7_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define IO_RC7_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define IO_RC7_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)

/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/