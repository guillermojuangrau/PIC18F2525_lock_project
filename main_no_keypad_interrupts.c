 /***** main *********************************************************
 *                                                                  *
 * Programa:                                                        *
 *                                                                  *
 * Autor: Antonio Ángel Pastor Valles                               *
 *        Guillermo de Juan Grau                                    *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/

#include <p18f2525.h> // Cabecera específica del procesador
#include "lock4.h"
#include "keypad.h"
#include "soundCCP2.h"
#include "leds.h"

#pragma config WDT=OFF, MCLRE = ON, DEBUG = ON, LVP = OFF, OSC = XT
#pragma config CCP2MX = PORTBE

#define F_OSC 8 // En MHz

void high_ISR(void);
void low_ISR(void);

/* Variable que indica el nº intentos erroneos restantes hasta bloqueo */
unsigned char retries_left = 3;

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
 *  Al saltar la interrupción, el programa chequea que el timeout
 * realmente ha expirado y de ser así, ejecuta la función decode_lock(),
 * ejecutando algo distinto conforme a lo que esta función devuelva,
 * según las especificaciones del programa.
 *
 *******************************************************************/
#pragma code
#pragma interrupt high_ISR
void high_ISR (void)
{
    if(check_lock_buffer_timeout())
    {
        switch (decode_lock())
        {
            case LOCK_PASSWORD_CHANGED:
                play_long_beep(F_OSC);
                break;
            case LOCK_OPENING:
                play_melody(1, F_OSC);
                set_ra5_clear_ra4(); // RA5 led VERDE
                break;
            case LOCK_CLOSING:
                set_ra4_clear_ra5(); // RA4 led ROJO
                break;
            case LOCK_WRONG_CLOSING_CODE_ENTERED:
                play_melody(2, F_OSC);
                if (retries_left)
                {
                    retries_left--;
                }
                else
                {
                    retries_left = 3;
                    blink_ra4_1minute(F_OSC);
                }
                break;
        }
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
 * en un bucle eterno en el que irá escaneando el teclado, ejecutando
 * la función destinada a añadir la tecla pulsada al buffer de entrada
 * de la cerradura y haciendo sonar un beep si detecta una pulsación.
 *******************************************************************/
void main (void)
{
    /* Configuración inicial */
    init_leds_ra4_ra5();
    setup_keypad();

    while(1)
    {
        if (get_key()!=0xFF)
        {
            add2lock_buffer(get_key(), F_OSC);
            play_beep(F_OSC);
        }
    }
}