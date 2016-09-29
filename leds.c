/***** leds.c *****************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio Ángel Pastor Valles                               *
 *        Guillermo de Juan Grau                                    *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/
#include "leds.h"
#include "delays_ms.h"

/***************************************** init_leds_ra4_ra5 () *****
 *  Función para configurar y ejecutar el estado inicial de los dos LEDs.
 *
 *  Configura los pines RA4 y RA5 como salidas y pone RA4 (LED rojo) en nivel bajo (apagado) 
 * y RA5 (LED verde) en nivel alto (encendido).
 *
 ********************************************************************/
void init_leds_ra4_ra5 (void)
{
    TRISAbits.RA4 = 0;
    TRISAbits.RA5 = 0;
    PORTAbits.RA4 = 0;  // Led Rojo
    PORTAbits.RA5 = 1;  // Led Verde
}

/************************************************* blink_ra4 () *****
 *  Función para que el LED rojo parpadee dos veces por minuto durante 1 minuto.
 *
 *  Pone el pin RA4 en nivel bajo y ejecuta un while que se repetira 120 veces, en cada una de ellas el LED
 * cambiará su estado y esperará medio segundo.
 *
 ********************************************************************/
void blink_ra4_1minute (char f_osc)
{
    unsigned char n_reps = 120; // 60 segundos / 0.5 segundos (de cada parpadeo)

    TRISAbits.RA4 = 0;
    PORTAbits.RA4 = 0;    

    while(n_reps--)
    {
        PORTAbits.RA4 ^= 1;
        wait_500ms(f_osc);
    }
    PORTAbits.RA4 = 1;
}

/***************************************** set_ra4_clear_ra5 () *****
 *
 * Función para activar el LED rojo y desactivar el LED verde.
 *
 ********************************************************************/
void set_ra4_clear_ra5 (void)
{

    PORTAbits.RA4 = 1;
    PORTAbits.RA5 = 0;
}

/***************************************** set_ra5_clear_ra4 () *****
 *
 * Función para desactivar el LED rojo y activar el LED verde.
 *
 ********************************************************************/
void set_ra5_clear_ra4 (void)
{

    PORTAbits.RA4 = 0;
    PORTAbits.RA5 = 1;
}
