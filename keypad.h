/***** keypad.h *****************************************************
 *                                                                  *
 * Biblioteca: Lectura de un teclado matricial hexadecimal para el  *
 *             microcontrolador PIC18f2525 de microchip.            *
 *                                                                  *
 *  Conectando las cuatro filas como salidas a los pines            *
 * RA0, RA1, RA2 y RA3; y las cuatro columnas como entradas a       *
 * RB0, RB1, RB2 y RB4, que al tener interrupciones configurables   *
 * permiten una configuraci�n que no tenga que leer el teclado      *
 * constantemente.                                                  *
 *                                                                  *
 *  El teclado se conectar� con una resistencia de pull-up en cada  *
 * fila y columna conectadas todas a la tensi�n de referencia.      *
 *                                                                  *
 * Se podr�a conectar con las resistencias de pull-up a masa si no  *
 * se va a hacer uso de las interrupciones.                         *
 *                                                                  *
 * Autor: Antonio �ngel Pastor Valles                               *
 *        Guillermo de Juan Grau                                    *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/
#ifndef _KEYPAD_
#define _KEYPAD_

#include <p18f2525.h> // Cabecera espec�fica del procesador

/********************************************* setup_keypad() *****
 *  Funci�n para configurar un teclado matricial hexadecimal (4x4)
 *
 *  Las filas (RA0, RA1, RA2 y RA3) son salidas
 * y las columnas (RB0, RB1, RB2 y RB4) entradas
 *
 *  RB3 se deja disponible as� para funcionar como CCP2, adem�s de
 * que RB3 no tiene interrupci�n externa y RB4 si.
 *
 *  El teclado se conectar� con una resistencia de pull-up en cada
 * fila y columna conectadas todas a la tensi�n de referencia.
 *
 ********************************************************************/
void setup_keypad (void);


/**************************************** setup_kp_interrupts() *****
 *  Funci�n para configurar un teclado matricial hexadecimal (4x4)
 *
 *  Para detectar la pulsaci�n de alguna tecla se pretende hacer uso
 * de las interrupciones RB0/INT0, RB1/INT1, RB2/INT2 (interrupciones
 * de flanco) y RB4/KBI0 (interrupci�n de cambio de nivel)
 *
 *  Se configuran todas las interrupciones con un nivel de prioridad
 * alto, ya que INT0 siempre es de prioridad alta.
 *
 *  Necesita que en todo momento se reconfigure las filas a nivel
 * alto, para detectar los flancos de subida y el cambio de nivel.
 *
 ********************************************************************/
void setup_kp_interrupts (void);


/************************************* get_key_from_interrupt() *****
 *   Funci�n para lectura de un teclado matricial hexadecimal (4x4)
 * de contactos met�licos, los cuales producen rebotes durante
 * al menos los primeros 5 ms de la pulsaci�n de la tecla.
 *
 *  Dise�ada para ser llamada desde la RAI al ser activada por pulsaci�n del
 * teclado. La funci�n espera 10 ms antes leer el teclado, desechando as� el
 * posible ruido. La pulsaci�n de una tecla con el dedo dura un minimo de 20 ms.
 *
 *  Devuelve un 'unsigned char' con el valor de la tecla pulsada.
 * Devuelve 0xFF si no se ha pulsado ninguna tecla
 *
 *  Par�metro de entrada 'f_osc' con la frecuencia del oscilador. Que se utiliza
 * en 'wait_10ms(f_osc)' para calcular el retardo de lectura del teclado.
 *
 *  Requiere 'setup_keyboard ()' y 'setup_kb_interrupts()'
 *
 *  Ver tambi�n 'get_key()' y 'wait_10ms(f_osc)'
 ********************************************************************/
unsigned char get_key_from_interrupt(char f_osc);


/**************************************************** get_key() *****
 *  Funci�n para lectura de un teclado matricial hexadecimal (4x4)
 *
 *  Devuelve un 'unsigned char' con el valor de la tecla pulsada.
 * Devuelve 0xFF si no se ha pulsado ninguna tecla
 *
 *  Requiere 'setup_keyboard ()'
 *
 *  Ver tambi�n 'get_key_debouncing()' para evitar rebotes de los
 * contactos met�licos
 ********************************************************************/
unsigned char get_key(void);


/********************************************* check_kp_flags() *****
 *  Funci�n que comprueba si alguno de los flags que se activan debido
 * a las interrupciones INT0, INT1, INT2 o KBI0 esta activa.
 *
 *
 ********************************************************************/
unsigned char check_kp_flags (void);


/************************************** disable_kp_interrupts() *****
 *  Funci�n que deshabilita las interrupciones usadas durante el programa 
 * mediante los registros INTCON3 e INTCON.
 *
 *
 ********************************************************************/
void disable_kp_interrupts (void);


/*************************************** check_kp_int_enabled() *****
 *  Funci�n que devuelve 1 si las interrupciones estan correctamente
 * configuradas y 0 si no lo est�n.
 *
 *
 ********************************************************************/
char check_kp_int_enabled (void);

#endif
