/***** delays_ms.c *****************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio �ngel Pastor Valles                               *
 *        Guillermo de Juan Grau                                    *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/

#include "delays_ms.h"

#include <delays.h>


/************************************************** wait_500ms() *****
 *  Funci�n para realizar un retardo de 500 milisegundos (medio segundo).
 *
 *  Se utiliza la biblioteca 'delays.h' del compilador MPLAB C18
 *
 *  Utiliza la f�rmula:
 *      T_oscilador = 1 / F_oscilador
 *      T_instrucci�n = 4 * T_oscilador
 *
 *      500 ms / T_instrucci�n = No ciclos de instrucci�n a esperar
 *
 *  Par�metro de entrada 'f_osc' con la frecuencia del oscilador.
 *
 *  Ver tambi�n 'Delay10KTCYx(unsigned char)' [delays.h C18 library]
 ********************************************************************/
void wait_500ms(char f_osc)
{
    short long n_ciclos = 500e-3 / (4*(1/f_osc));
    
    Delay10KTCYx((unsigned char)n_ciclos/10000);
}


/************************************************** wait_10ms() *****
 *  Funci�n para realizar un retardo de 10 ms.
 *
 *  Se utiliza la biblioteca 'delays.h' del compilador MPLAB C18
 *
 *  Utiliza la f�rmula:
 *      T_oscilador = 1 / F_oscilador
 *      T_instrucci�n = 4 * T_oscilador
 *
 *      10 ms / T_instrucci�n = No ciclos de instrucci�n a esperar
 *
 *  Par�metro de entrada 'f_osc' con la frecuencia del oscilador.
 *
 *  Ver tambi�n 'Delay1KTCYx(unsigned char)' [delays.h C18 library]
 ********************************************************************/
void wait_10ms(char f_osc)
{
    int n_ciclos = 10e-3 / (4*(1/f_osc));
    Delay1KTCYx((unsigned char)n_ciclos/1000);
}


/************************************************** wait_5ms() *****
 *  Funci�n para realizar un retardo de 5 ms.
 *
 *  Se utiliza la biblioteca 'delays.h' del compilador MPLAB C18
 *
 *  Utiliza la f�rmula:
 *      T_oscilador = 1 / F_oscilador
 *      T_instrucci�n = 4 * T_oscilador
 *
 *      5 ms / T_instrucci�n = No ciclos de instrucci�n a esperar
 *
 *  Par�metro de entrada 'f_osc' con la frecuencia del oscilador.
 *
 *  Ver tambi�n 'Delay1KTCYx(unsigned char)' [delays.h C18 library]
 ********************************************************************/
void wait_5ms(char f_osc)
{
    int n_ciclos = 5e-3 / (4*(1/f_osc));
    Delay1KTCYx((unsigned char)n_ciclos/1000);
}
