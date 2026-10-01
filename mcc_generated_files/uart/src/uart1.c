/**
 * UART1 Generated Driver API Header File
 * 
 * @file uart1.c
 * 
 * @ingroup uart1
 * 
 * @brief This is the generated driver implementation file for the UART1 driver using the Universal Asynchronous Receiver and Transmitter (UART) module.
 *
 * @version UART1 Driver Version 3.0.9
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
  Section: Included Files
*/
#include "../uart1.h"
#include <stdint.h>
#include <stdarg.h>

/**
  Section: Macro Declarations
*/

/**
  Section: UART1 variables
*/
 /**
 * @misradeviation{@advisory,19.2}
 * The UART error status necessitates checking the bitfield and accessing the status within the group byte therefore the use of a union is essential.
 */
 /* cppcheck-suppress misra-c2012-19.2 */
static volatile uart1_status_t uart1RxLastError;

/**
  Section: UART1 APIs
*/

static void (*UART1_FramingErrorHandler)(void);
static void (*UART1_OverrunErrorHandler)(void);
static void (*UART1_ParityErrorHandler)(void);

static void UART1_DefaultFramingErrorCallback(void);
static void UART1_DefaultOverrunErrorCallback(void);
static void UART1_DefaultParityErrorCallback(void);

/**
  Section: UART1  APIs
*/

void UART1_Initialize(void)
{

    // Set the UART1 module to the options selected in the user interface.

    //RXCHK disabled; 
    U1RXCHK = 0x0;
    //TXCHK disabled; 
    U1TXCHK = 0x0;
    //P1L 0x0; 
    U1P1L = 0x0;
    //P1H 0x0; 
    U1P1H = 0x0;
    //P2L 0x0; 
    U1P2L = 0x0;
    //P2H 0x0; 
    U1P2H = 0x0;
    //P3L 0x0; 
    U1P3L = 0x0;
    //P3H 0x0; 
    U1P3H = 0x0;
    //MODE Asynchronous 8-bit mode; RXEN enabled; TXEN enabled; ABDEN disabled; BRGS high speed; 
    U1CON0 = 0xB0;
    //SENDB disabled; BRKOVR disabled; RXBIMD Set RXBKIF on rising RX input; WUE disabled; ON enabled; 
    U1CON1 = 0x80;
    //FLO off; TXPOL not inverted; C0EN Add all TX and RX characters; STP Transmit 1Stop bit, receiver verifies first Stop bit; RXPOL not inverted; RUNOVF RX input shifter stops all activity; 
    U1CON2 = 0x8;
    //BRGL 138; 
    U1BRGL = 0x8A;
    //BRGH 0; 
    U1BRGH = 0x0;
    //TXBE empty; STPMD in middle of first Stop bit; TXWRE No error; 
    U1FIFO = 0x2E;
    //ABDIE disabled; ABDIF Auto-baud not enabled or not complete; WUIF WUE not enabled by software; 
    U1UIR = 0x0;
    //TXCIF equal; RXFOIF not overflowed; RXBKIF No Break detected; FERIF no error; CERIF No Checksum error; ABDOVF Not overflowed; PERIF no parity error; TXMTIF empty; 
    U1ERRIR = 0x80;
    //TXCIE disabled; RXFOIE disabled; RXBKIE disabled; FERIE disabled; CERIE disabled; ABDOVE disabled; PERIE disabled; TXMTIE disabled; 
    U1ERRIE = 0x0;

    UART1_FramingErrorCallbackRegister(UART1_DefaultFramingErrorCallback);
    UART1_OverrunErrorCallbackRegister(UART1_DefaultOverrunErrorCallback);
    UART1_ParityErrorCallbackRegister(UART1_DefaultParityErrorCallback);

    uart1RxLastError.status = 0;
    
    UART1_TransmitEnable();
    UART1_ReceiveEnable();
    UART1_Enable();
}

void UART1_Deinitialize(void)
{
    U1RXB = 0x00;
    U1RXCHK = 0x00;
    U1TXB = 0x00;
    U1TXCHK = 0x00;
    U1P1L = 0x00;
    U1P1H = 0x00;
    U1P2L = 0x00;
    U1P2H = 0x00;
    U1P3L = 0x00;
    U1P3H = 0x00;
    U1CON0 = 0x00;
    U1CON1 = 0x00;
    U1CON2 = 0x00;
    U1BRGL = 0x00;
    U1BRGH = 0x00;
    U1FIFO = 0x00;
    U1UIR = 0x00;
    U1ERRIR = 0x00;
    U1ERRIE = 0x00;
}

void UART1_Enable(void)
{
    U1CON1bits.ON = 1; 
}

void UART1_Disable(void)
{
    U1CON1bits.ON = 0; 
}

void UART1_TransmitEnable(void)
{
    U1CON0bits.TXEN = 1;
}

void UART1_TransmitDisable(void)
{
    U1CON0bits.TXEN = 0;
}

void UART1_ReceiveEnable(void)
{
    U1CON0bits.RXEN = 1;
}

void UART1_ReceiveDisable(void)
{
    U1CON0bits.RXEN = 0;
}

void UART1_SendBreakControlEnable(void)
{
    U1CON1bits.SENDB = 1;
}

void UART1_SendBreakControlDisable(void)
{
    U1CON1bits.SENDB = 0;
}

void UART1_AutoBaudSet(bool enable)
{
    if(enable)
    {
        U1CON0bits.ABDEN = 1; 
    }
    else
    {
      U1CON0bits.ABDEN = 0;  
    }
}


bool UART1_AutoBaudQuery(void)
{
    return (bool)U1UIRbits.ABDIF; 
}

void UART1_AutoBaudDetectCompleteReset(void)
{
    U1UIRbits.ABDIF = 0; 
}

bool UART1_IsAutoBaudDetectOverflow(void)
{
    return (bool)U1ERRIRbits.ABDOVF; 
}

void UART1_AutoBaudDetectOverflowReset(void)
{
    U1ERRIRbits.ABDOVF = 0; 
}

bool UART1_IsRxReady(void)
{
    return (bool)(!U1FIFObits.RXBE);
}

bool UART1_IsTxReady(void)
{
    return (bool)(U1FIFObits.TXBE && U1CON0bits.TXEN);
}

bool UART1_IsTxDone(void)
{
    return U1ERRIRbits.TXMTIF;
}

size_t UART1_ErrorGet(void)
{
    uart1RxLastError.status = 0;
    
    if(true == U1ERRIRbits.FERIF)
    {
        uart1RxLastError.ferr = 1;
        if(NULL != UART1_FramingErrorHandler)
        {
            UART1_FramingErrorHandler();
        }  
    }
    if(true == U1ERRIRbits.RXFOIF)
    {
        uart1RxLastError.oerr = 1;
        if(NULL != UART1_OverrunErrorHandler)
        {
            UART1_OverrunErrorHandler();
        }   
    }
    if(true == U1ERRIRbits.PERIF)
    {
        uart1RxLastError.perr = 1;
        if(NULL != UART1_ParityErrorHandler)
        {
            UART1_ParityErrorHandler();
        }   
    }

    return uart1RxLastError.status;
}

uint8_t UART1_Read(void)
{
    return U1RXB;
}


void UART1_Write(uint8_t txData)
{
    // Wait until Transmit FIFO is ready to accept a new byte
    while (!UART1_IsTxReady()); 
    U1TXB = txData; 
}

void UART1_WriteString(const char* str) 
{
    while (*str != '\0') {
        UART1_Write(*str); // Now safely waits for each character!
        str++;
    }
}



static void UART1_DefaultFramingErrorCallback(void)
{
    
}

static void UART1_DefaultOverrunErrorCallback(void)
{
    
}

static void UART1_DefaultParityErrorCallback(void)
{
    
}

void UART1_FramingErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        UART1_FramingErrorHandler = callbackHandler;
    }
}

void UART1_OverrunErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        UART1_OverrunErrorHandler = callbackHandler;
    }    
}

void UART1_ParityErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        UART1_ParityErrorHandler = callbackHandler;
    } 
}

/* ---------------------------------------------------------
 * Convert unsigned integer to decimal/hex/binary string
 * --------------------------------------------------------- */
void UART1_WriteUnsigned(unsigned long value, uint8_t base_val)
{
    char buffer[33];
    uint8_t i = 0;
    char digit;

    if (value == 0)
    {
        UART1_Write('0');
        return;
    }

    while (value > 0)
    {
        uint8_t remainder = value % base_val;

        if (remainder < 10)
            digit = '0' + remainder;
        else
            digit = 'A' + (remainder - 10);

        buffer[i++] = digit;
        value /= base_val;
    }

    while (i > 0)
    {
        UART1_Write(buffer[--i]);
    }
}

/* ---------------------------------------------------------
 * Signed integer
 * --------------------------------------------------------- */
void UART1_WriteSigned(long value)
{
    if (value < 0)
    {
        UART1_Write('-');

        /* Avoid overflow for LONG_MIN */
        UART1_WriteUnsigned((unsigned long)(-(value + 1)) + 1, 10);
    }
    else
    {
        UART1_WriteUnsigned((unsigned long)value, 10);
    }
}

/* ---------------------------------------------------------
 * Custom printf
 * --------------------------------------------------------- */
void UART1_Printf(const char *format, ...)
{
    va_list args;
    char c;
    char *str;

    va_start(args, format);

    while (*format != '\0')
    {
        if (*format != '%')
        {
            UART1_Write(*format);
            format++;
            continue;
        }

        format++;

        /* Handle %% */
        if (*format == '%')
        {
            UART1_Write('%');
        }

        /* Character */
        else if (*format == 'c')
        {
            c = (char)va_arg(args, int);
            UART1_Write(c);
        }

        /* String */
        else if (*format == 's')
        {
            str = va_arg(args, char *);

            if (str != 0)
            {
                UART1_WriteString(str);
            }
        }

        /* Signed integer */
        else if (*format == 'd' || *format == 'i')
        {
            UART1_WriteSigned((long)va_arg(args, int));
        }

        /* Unsigned integer */
        else if (*format == 'u')
        {
            UART1_WriteUnsigned(
                (unsigned long)va_arg(args, unsigned int),
                10
            );
        }

        /* Hexadecimal */
        else if (*format == 'x' || *format == 'X')
        {
            UART1_WriteUnsigned(
                (unsigned long)va_arg(args, unsigned int),
                16
            );
        }

        /* Binary */
        else if (*format == 'b')
        {
            UART1_WriteUnsigned(
                (unsigned long)va_arg(args, unsigned int),
                2
            );
        }

        /* Long integer */
        else if (*format == 'l')
        {
            format++;

            if (*format == 'd' || *format == 'i')
            {
                UART1_WriteSigned(va_arg(args, long));
            }
            else if (*format == 'u')
            {
                UART1_WriteUnsigned(va_arg(args, unsigned long), 10);
            }
            else if (*format == 'x' || *format == 'X')
            {
                UART1_WriteUnsigned(va_arg(args, unsigned long), 16);
            }
        }

        format++;
    }

    va_end(args);
}

/**
 * @brief Prints an unsigned 16-bit number (0 to 65535) as decimal text over UART1.
 * @param value The 16-bit number to display.
 */
void UART_PrintDec(uint16_t value) {
    char buf[6];
    int8_t i = 0;

    // Special case for zero
    if (value == 0) {
        UART1_Write('0');
        return;
    }

    // Extract digits in reverse order
    while (value > 0 && i < 5) {
        buf[i++] = (char)('0' + (value % 10));
        value /= 10;
    }

    // Send digits in correct order (highest place-value first)
    while (--i >= 0) {
        UART1_Write(buf[i]);
    }
}

/**
 * @brief (Optional) Prints a signed 16-bit number (-32768 to +32767) over UART1.
 * Useful if you want to inspect raw signed differentials (e.g. ADERR).
 */
void UART_PrintSignedDec(int16_t value) {
    if (value < 0) {
        UART1_Write('-');
        value = -value;
    }
    UART_PrintDec((uint16_t)value);
}

