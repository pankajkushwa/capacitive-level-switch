

#include "rotary_switch.h"
#include "mcc_generated_files/system/pins.h"
#include "stdbool.h"
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/uart/uart1.h"

static ROTARY_SWITCH_t rotary;



static uint8_t Rotary_ReadRaw(void)
{
    uint8_t value = 0;

    value |= Rotary_SW1_GetValue();
    value |= Rotary_SW2_GetValue();
    value |= Rotary_SW4_GetValue();
    value |= Rotary_SW8_GetValue();

    return value;
}

void Rotary_Init(void)
{
    rotary.currentPosition = Rotary_ReadRaw();
    rotary.previousPosition = rotary.currentPosition;
    rotary.positionChanged = false;
    
#ifdef DEBUG_ENABLE
    UART1_Printf("Rotary Position: %d", rotary.currentPosition);
#endif
}


void Rotary_Task(void)
{
    uint8_t pos = Rotary_ReadRaw();

    /* Valid BCD positions are 0-9 */
    if(pos <= 9)
    {
        if(pos != rotary.currentPosition)
        {
            rotary.previousPosition = rotary.currentPosition;
            rotary.currentPosition = pos;
            rotary.positionChanged = true;
        }
    }
}

uint8_t Rotary_GetPosition(void)
{
    return rotary.currentPosition;
}

bool Rotary_PositionChanged(void)
{
    bool state = rotary.positionChanged;
    rotary.positionChanged = false;
    return state;
}

void Rotary_Set_Permitivity_Range(void)
{
    if(Rotary_PositionChanged())
    {
        uint8_t position = Rotary_GetPosition();

        switch(position)
        {
            case 0:
#ifdef DEBUG_ENABLE
                UART1_WriteString("Zero position.\r\n");
#endif
                break;

            case 1:
#ifdef DEBUG_ENABLE
                UART1_WriteString("One position.\r\n");
#endif
                break;

            case 2:
#ifdef DEBUG_ENABLE
                UART1_WriteString("Two position.\r\n");
#endif
                break;

            case 3:
#ifdef DEBUG_ENABLE
                UART1_WriteString("Three position.\r\n");
#endif
                break;

            case 4:
#ifdef DEBUG_ENABLE
                UART1_WriteString("Four position.\r\n");
#endif
                break;

            case 5:
#ifdef DEBUG_ENABLE
                UART1_WriteString("Five position.\r\n");
#endif
                break;

            case 6:
#ifdef DEBUG_ENABLE
                UART1_WriteString("Six position.\r\n");
#endif
                break;

            case 7:
#ifdef DEBUG_ENABLE
                UART1_WriteString("Seven position.\r\n");
#endif
                break;

            case 8:
#ifdef DEBUG_ENABLE
                UART1_WriteString("Eight position.\r\n");
#endif
                break;

            case 9:
#ifdef DEBUG_ENABLE
                UART1_WriteString("Nine position.\r\n");
#endif
                break;
        }
    }
}



