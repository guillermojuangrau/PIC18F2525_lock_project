/***** delays_ms.h **************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio Ángel Pastor Valles                               *
 *                                                                  *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/

#ifndef _DELAYS_MS_
#define _DELAYS_MS_


/************************************************* wait_500ms() *****
 *  Función para realizar un retardo de 500 milisegundos (medio segundo).
 *
 *  Se utiliza la biblioteca 'delays.h' del compilador MPLAB C18
 *
 *  Utiliza la fórmula:
 *      T_oscilador = 1 / F_oscilador
 *      T_instrucción = 4 * T_oscilador
 *
 *      500 ms / T_instrucción = No ciclos de instrucción a esperar
 *
 *  Parámetro de entrada 'f_osc' con la frecuencia del oscilador.
 *
 *  Ver también 'Delay10KTCYx(unsigned char)' [delays.h C18 library]
 ********************************************************************/
void wait_500ms(char f_osc);

/************************************************** wait_10ms() *****
 *  Función para realizar un retardo de 10 ms.
 *
 *  Se utiliza la biblioteca 'delays.h' del compilador MPLAB C18
 *
 *  Utiliza la fórmula:
 *      T_oscilador = 1 / F_oscilador
 *      T_instrucción = 4 * T_oscilador
 *
 *      10 ms / T_instrucción = No ciclos de instrucción a esperar
 *
 *  Parámetro de entrada 'f_osc' con la frecuencia del oscilador.
 *
 *  Ver también 'Delay1KTCYx(unsigned char)' [delays.h C18 library]
 ********************************************************************/
void wait_10ms(char f_osc);


/************************************************** wait_5ms() *****
 *  Función para realizar un retardo de 5 ms.
 *
 *  Se utiliza la biblioteca 'delays.h' del compilador MPLAB C18
 *
 *  Utiliza la fórmula:
 *      T_oscilador = 1 / F_oscilador
 *      T_instrucción = 4 * T_oscilador
 *
 *      5 ms / T_instrucción = No ciclos de instrucción a esperar
 *
 *  Parámetro de entrada 'f_osc' con la frecuencia del oscilador.
 *
 *  Ver también 'Delay1KTCYx(unsigned char)' [delays.h C18 library]
 ********************************************************************/
void wait_5ms(char f_osc);

#endif
