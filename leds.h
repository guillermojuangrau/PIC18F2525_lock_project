/***** leds.h *****************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio Ángel Pastor Valles                               *
 *        Guillermo de Juan Grau                                    *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/

#ifndef _LEDS_
#define _LEDS_

#include <p18f2525.h> // Cabecera específica del procesador

/***************************************** init_leds_ra4_ra5 () *****
 *  Función para configurar y ejecutar el estado inicial de los dos LEDs.
 *
 *  Configura los pines RA4 y RA5 como salidas y pone RA4 (LED rojo) en nivel bajo (apagado) 
 * y RA5 (LED verde) en nivel alto (encendido).
 *
 ********************************************************************/
void init_leds_ra4_ra5 (void);


/************************************************* blink_ra4 () *****
 *  Función para que el LED rojo parpadee dos veces por minuto durante 1 minuto.
 *
 ********************************************************************/
void blink_ra4_1minute (char f_osc);


/***************************************** set_ra4_clear_ra5 () *****
 *
 * Función para activar el LED rojo y desactivar el LED verde.
 *
 ********************************************************************/
void set_ra4_clear_ra5 (void);


/***************************************** set_ra5_clear_ra4 () *****
 *
 * Función para desactivar el LED rojo y activar el LED verde.
 *
 ********************************************************************/
void set_ra5_clear_ra4 (void);

#endif
