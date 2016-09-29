/***** lock4_usart.c ******************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio Ángel Pastor Valles                               *
 *                                                                  *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/
#include "lock4_usart.h"
#include "lock4.h"
#include <timers.h>
#include <usart.h>


/*
 * DECLARACIÓN DE VARIABLES INTERNAS
 */

unsigned int timeout_reps = 0;
unsigned int timeout_threshold = 0;

/* Siguiente posición del buffer vacía */
unsigned char current_usart_buffer_position = 0;

/*
 *  El codigo es de 4 cifras, la posición 0 es para el tipo de operación,
 * las posiciones 1, 2, 3 y 4 son para el codigo, la posición 5 sirve
 * para detectar intentos de código más largos de 4 (erróneos por longitud)
 */
unsigned char lock_usart_buffer[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


/******************************************* setup_lock_usart() *****
 *
 *
 *
 *
 ********************************************************************/
void setup_lock_usart (int f_osc)
{
    /* spbrg = ((f_osc/Desired Baud Rate)/64) ? 1 */
    int spbrg = ((f_osc/9600)/64)-1;

    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    OpenUSART( USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE &
            USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, spbrg);
    /* ¿?
     baudUSART (BAUD_IDLE_CLK_HIGH & BAUD_16_BIT_RATE & BAUD_WAKEUP_ON &
            BAUD_AUTO_ON);
     */
}


/***************************************** disable_lock_usart() *****
 *
 *
 *
 *
 ********************************************************************/
void disable_lock_usart (void)
{
    CloseUSART();
}

/************************************** add2lock_usart_buffer() *****
 *
 *
 *  !!!! Recibimos carácteres ascii!!!!!!!!!!!!!!!!!! TRANSFORMAR!!!
 *
 ********************************************************************/
char add2lock_usart_buffer(char f_osc)
{
    start_lock_usart_buffer_timeout(f_osc);

    if(current_usart_buffer_position<6)
    {
        lock_usart_buffer[current_usart_buffer_position] = ReadUSART();
        current_usart_buffer_position++;
        return 0; // 0 = añadido correctamente
    }else
    {
        return 1; // 1 = buffer lleno, imposible añadir
    }
}


/****************************************** decode_lock_usart() *****
 *
 *
 *
 *
 ********************************************************************/
char decode_lock_usart(void)
{
    char operation = LOCK_UNCHANGED;

    CloseTimer1(); // Deshabilita la cuenta del timeout

    if (current_usart_buffer_position==0) // Buffer vacio
        return operation;

    decode_lock_external_buffer(lock_usart_buffer[0], lock_usart_buffer[1],
            lock_usart_buffer[2], lock_usart_buffer[3], lock_usart_buffer[4],
            lock_usart_buffer[5]);

    empty_lock_usart_buffer();
    return operation;
}


/****************************************** decode_lock_usart() *****
 *
 *
 *
 *
 ********************************************************************/
char ascii2lock_raw (char ascii)
{
    switch (ascii)
    {
        case 'A':
        case 'a':
            return 0x0A;

        case 'B':
        case 'b':
            return 0x0B;

        case 'C':
        case 'c':
            return 0x0C;

        case '0':
            return 0x00;

        case '1': return 0x01;
        case '2': return 0x02;
        case '3': return 0x03;
        case '4': return 0x04;
        case '5': return 0x05;
        case '6': return 0x06;
        case '7': return 0x07;
        case '8': return 0x08;
        case '9': return 0x09;
        default: return 0x10; // Caracter no asignado a ninguna tecla
    }
}

/************************************ empty_lock_usart_buffer() *****
 *
 *
 *
 *
 ********************************************************************/
void empty_lock_usart_buffer(void)
{
    lock_usart_buffer[0] = 0xFF;
    lock_usart_buffer[1] = 0xFF;
    lock_usart_buffer[2] = 0xFF;
    lock_usart_buffer[3] = 0xFF;
    lock_usart_buffer[4] = 0xFF;
    lock_usart_buffer[5] = 0xFF;
}


/********************************** start_lock_usart_buffer_timeout() *****
 *
 *
 *
 *
 ********************************************************************/
void start_lock_usart_buffer_timeout(char f_osc)
{
    timeout_threshold = (unsigned int)(LOCK_USART_TIMEOUT/((8/f_osc)*2^16))+1;
    timeout_reps = 0;
    
    INTCONbits.GIEH = 1;
    OpenTimer1(TIMER_INT_ON && T1_16BIT_RW && T1_SOURCE_INT && T1_PS_1_8);
    WriteTimer1 (0x0000);
    PIR1bits.TMR1IF = 0;
}


/**************************** check_lock_usart_buffer_timeout() *****
 *
 *
 ********************************************************************/
unsigned char check_lock_usart_buffer_timeout(void)
{
    if (PIR1bits.TMR1IF)
    {
        PIR1bits.TMR1IF = 0;
        if (timeout_reps == timeout_threshold)
        {
            timeout_reps = 0;
            return 1;
        }
        else
        {
            timeout_reps++;
        }
    }
        return 0;
}
