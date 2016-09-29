/***** soundCCP2.h **************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio Ángel Pastor Valles                               *
 *        Guillermo de Juan Grau                                    *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/

#ifndef _SOUNDCCP2_
#define _SOUNDCCP2_

#include <p18f2525.h> // Cabecera específica del procesador

/************************************************ setup_sound() *****
 *  Función que configura las salidas para poder usar el altavoz como periferico.
 *
 *
 *
 ********************************************************************/
void setup_sound (void);


/************************************************ disable_sound() *****
 *  Función que deshabilita el sonido.
 *
 *
 ********************************************************************/
void disable_sound (void);


/************************************************** play_beep() *****
 *  Función que reproduce un beep, recibiendo como variable la frecuencia
 * del oscilador.
 *
 *
 ********************************************************************/
void play_beep (char f_osc);


/********************************************* play_long_beep() *****
 *  Función que reproduce un beep de mayor longitud en el tiempo 
 * que el anterior.
 *
 *
 ********************************************************************/
void play_long_beep (char f_osc);


/************************************************ play_melody() *****
 *   Función que recibe como parametros un numero y la frecuencia del
 *  oscilador. Esta función reproduce la melodia nº X almacenada en la
 *  memoria del microprocesador, siendo X el numero pasado como parametro
 * (en este caso solo hay dos melodias almacenadas).
 *
 ********************************************************************/
void play_melody (unsigned char n_melody, char f_osc);


#endif
