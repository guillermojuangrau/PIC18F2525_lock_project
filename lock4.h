/***** lock4.h ******************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio �ngel Pastor Valles                               *
 *        Guillermo de Juan Grau				    *
 *								    *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/
#ifndef _LOCK4_
#define _LOCK4_

#include <p18f2525.h> // Cabecera espec�fica del procesador

#define LOCK_OPENED     1
#define LOCK_CLOSED     2

#define LOCK_UNCHANGED                  0
#define LOCK_OPENING                    3
#define LOCK_CLOSING                    4
#define LOCK_PASSWORD_CHANGED           5
#define LOCK_WRONG_CLOSING_CODE_ENTERED 6


#define LOCK_TIMEOUT 3 // En segundos (M�ximo 8 segundos a F_OSC = 8 MHz)


/******************************************** add2lock_buffer() *****
 *
 *  Funci�n que resetea el TMR0 y comprueba que el buffer no esta lleno,
 * si es as� introduce la ultima tecla pulsada (pasada a la funci�n como
 * par�metro) y devuelve 0, si no, devuelve 1 y descarta el numero. 
 *
 ********************************************************************/
char add2lock_buffer(unsigned char new, char f_osc);


/************************************************ decode_lock() *****
 *
 *   Funci�n que chequea y procesa variables tales como el estado actual
 * de la cerradura como las pulsaciones del teclado para cerrar/abrir
 * la cerradura o cambiar la contrase�a, etc...
 *
 ********************************************************************/
char decode_lock(void);


/******************************** decode_lock_external_buffer() *****
 *
 *   Esta funcion devuelve un char y recibe como par�metros de entrada
 * todos los elementos que se almacenan en un buffer de la cerradura.
 *
 *  Crea la variable "operation" donde se asigna el cambio de estado
 * de la cerradura al final del metodo. Para acometer dicha tarea la
 * funcion lee el estado actual (abierto=LOCK_OPENED, cerrado = LOCK_CLOSED, etc.)
 * mediante la variable lock_state.
 *
 *  Dependiendo del estado actual performar� varios chequeos para comprobar
 * si alguna de las combinaciones de teclas activa alguna acci�n disponible
 * (por ejemplo, si la cerradura esta abierta y el buffer de entrada solo ha
 * registrado una pulsaci�n a la tecla C, el microprocesador proceder� a cerrarse),
 * dependiendo de las opciones cambiar� el estado de la cerradura (lock_state)
 * y el valor de la variable "operation". Finalmente, devolver� esta.
 * 
 ********************************************************************/
char decode_lock_external_buffer(char b0, char b1, char b2, char b3,
        char b4, char b5);


/****************************************** empty_lock_buffer() *****
 *
 *   Funci�n que vacia el buffer de entrada a base de reescribir los valores
 * contenidos en �l por el valor por defecto (0xFF).
 *
 ********************************************************************/
void empty_lock_buffer(void);


/********************************** start_lock_buffer_timeout() *****
 *
 *   Funci�n que inicia el timeout del buffer (est� funcion se usar� tras
 * haber pulsado una tecla).
 *
 ********************************************************************/
void start_lock_buffer_timeout(char f_osc);


/********************************** check_lock_buffer_timeout() *****
 *
 *   Funci�n que chequea si el TMR0 ha saltado activando el flag, de ser
 * as� devuelve 1 y resetea el flag, en caso contrario devuelve 0.
 * 
 ********************************************************************/
unsigned char check_lock_buffer_timeout(void);


/********************************************* get_lock_state() *****
 *
 *   Funci�n que devuelve el estado de la cerradura, abierta o cerrada.
 *
 ********************************************************************/
char get_lock_state(void);

#endif
