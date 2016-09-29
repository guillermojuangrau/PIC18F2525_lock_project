/***** main *********************************************************
 *                                                                  *
 * Programa:                                                        *
 *                                                                  *
 * Autor: Antonio Ángel Pastor Valles                               *
 *        Guillermo de Juan Grau                                                          *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/

#include <p18f2525.h> // Cabecera específica del procesador
#include "usart.h"
#include "lock4.h"
#include "lock4_usart.h"
#include "keypad.h"
#include "soundCCP2.h"
#include "leds.h"


#pragma config WDT=OFF, MCLRE = ON, DEBUG = ON, LVP = OFF, OSC = XT
#pragma config CCP2MX = PORTBE

#define F_OSC 8 // En MHz
#define MAX_RETRIES 3

void high_ISR(void);
void low_ISR(void);

/* Variable que indica el nº de beeps a emitir */
unsigned char beeps_left = 0;

/* Variable que indica el nº intentos erroneos restantes hasta bloqueo */
unsigned char retries_left = MAX_RETRIES;

/* Variable que indica la última operación realizada */
unsigned char last_operation = LOCK_UNCHANGED;

#pragma code high_vector = 0x08
void high_interrupt (void)
{
	_asm
	goto high_ISR
	_endasm
}

#pragma code low_vector = 0x18
void low_interrupt (void)
{
	_asm
	goto low_ISR
	_endasm
}
/*****RAI*********************************************************
*  Al saltar la interrupción, el programa chequea si la interrupcion
* proviene del teclado, si es así introduce la tecla pulsada en el buffer
* de entrada, si no comprueba si el timeout ha expirado y de ser así, 
* ejecuta la función decode_lock(), ejecutando algo distinto conforme 
* a lo que esta función devuelva, según las especificaciones del programa.
*
*******************************************************************/
#pragma code
#pragma interrupt high_ISR
void high_ISR (void)
{
    if (check_kp_flags ())
    {
        add2lock_buffer(get_key_from_interrupt(F_OSC), F_OSC);
        beeps_left++;
    }
    else if (DataRdyUSART())
    {
        add2lock_usart_buffer(F_OSC);
    }
    else if (check_lock_buffer_timeout())
    {
        last_operation = decode_lock();
    }
    else if (check_lock_usart_buffer_timeout())
    {
        last_operation = decode_lock_usart();
    }
	
    _asm
	retfie 0
    _endasm
}

#pragma code
#pragma interrupt low_ISR
void low_ISR (void)
{
     _asm
	retfie 0
     _endasm
}

 /*****main()*********************************************************
 *  Configura los LEDs en la configuración inicial y configura el teclado
 * usando las fucniones especificadas para ello. Posteriormente entra
 * en un bucle eterno en el que comprueba si tiene beeps pendientes
 * para emitir por el altavoz y reproduciendolos.
 *******************************************************************/
void main (void)
{
    /* Configuración inicial */
    init_leds_ra4_ra5();
    setup_kp_interrupts();
    setup_lock_usart (F_OSC);

    while(1)
    {
       if(beeps_left)
       {
           play_beep(F_OSC);
           beeps_left--;
       }
       if(last_operation != LOCK_UNCHANGED)
       {
            switch (last_operation)
            {
            case LOCK_PASSWORD_CHANGED:
                putsUSART("Código de cerradura cambiado con éxito");
                play_long_beep(F_OSC);
                break;
            case LOCK_OPENING:
                putsUSART("Cerradura abierta");
                play_melody(1, F_OSC);
                set_ra5_clear_ra4(); // RA5 led VERDE
                retries_left = MAX_RETRIES;
                break;
            case LOCK_CLOSING:
                putsUSART("Cerradura cerrada");
                set_ra4_clear_ra5(); // RA4 led ROJO
                break;
            case LOCK_WRONG_CLOSING_CODE_ENTERED:
                putsUSART("Codigo incorrecto");
                play_melody(2, F_OSC);

                if (retries_left)
                {
                    retries_left--;
                }
                else
                {
                    putsUSART("3 intentos erroneos consecutivos. Cerradura bloqueada");
                    retries_left = MAX_RETRIES;
                    disable_kp_interrupts ();
                    disable_lock_usart ();
                    blink_ra4_1minute(F_OSC);
                    putsUSART("Fin del bloqueo");
                    setup_kp_interrupts();
                    setup_lock_usart (F_OSC);
                }
                break;
            }
       }
       last_operation = LOCK_UNCHANGED;
    }
}