/***** soundCCP2.h **************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio �ngel Pastor Valles                               *
 *        Guillermo de Juan Grau                                    *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/

#ifndef _SOUNDCCP2_
#define _SOUNDCCP2_

#include <p18f2525.h> // Cabecera espec�fica del procesador

/************************************************ setup_sound() *****
 *  Funci�n que configura las salidas para poder usar el altavoz como periferico.
 *
 *
 *
 ********************************************************************/
void setup_sound (void);


/************************************************ disable_sound() *****
 *  Funci�n que deshabilita el sonido.
 *
 *
 ********************************************************************/
void disable_sound (void);


/************************************************** play_beep() *****
 *  Funci�n que reproduce un beep, recibiendo como variable la frecuencia
 * del oscilador.
 *
 *
 ********************************************************************/
void play_beep (char f_osc);


/********************************************* play_long_beep() *****
 *  Funci�n que reproduce un beep de mayor longitud en el tiempo 
 * que el anterior.
 *
 *
 ********************************************************************/
void play_long_beep (char f_osc);


/************************************************ play_melody() *****
 *   Funci�n que recibe como parametros un numero y la frecuencia del
 *  oscilador. Esta funci�n reproduce la melodia n� X almacenada en la
 *  memoria del microprocesador, siendo X el numero pasado como parametro
 * (en este caso solo hay dos melodias almacenadas).
 *
 ********************************************************************/
void play_melody (unsigned char n_melody, char f_osc);


#endif
