/***** lock4.c ******************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio �ngel Pastor Valles                               *
 *        Guillermo de Juan Grau                                    *
 *                                                                  *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/
#include "lock4.h"

#include <timers.h>

/*
 * DECLARACI�N DE VARIABLES INTERNAS
 */

/* Siguiente posici�n del buffer vac�a */
unsigned char current_buffer_position = 0;

/* N�mero de intentos erroneos realizados */
unsigned char retries = 0;

/* Estado de la cerradura: LOCK_OPENED = abierto, LOCK_CLOSED = cerrado */
unsigned char lock_state = LOCK_OPENED;

/*
 *  El codigo es de 4 cifras, la posici�n 0 es para el tipo de operaci�n,
 * las posiciones 1, 2, 3 y 4 son para el codigo, la posici�n 5 sirve
 * para detectar intentos de c�digo m�s largos de 4 (err�neos por longitud)
 */
unsigned char lock_buffer[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/* Clave de la cerradura (4 digitos num�ricos) */
unsigned char lock_code[4] = {0, 0, 0, 0};


/******************************************** add2lock_buffer() *****
 *
 *   Funci�n que resetea el TMR0 y comprueba que el buffer no esta lleno,
 * si es as� introduce la ultima tecla pulsada (pasada a la funci�n como
 * par�metro) y devuelve 0, si no, devuelve 1 y descarta el numero. 
 *
 ********************************************************************/
char add2lock_buffer(unsigned char new, char f_osc)
{
    start_lock_buffer_timeout(f_osc);
    
    if(current_buffer_position<6)
    {
        lock_buffer[current_buffer_position] = new;
        current_buffer_position++;
        return 0; // 0 = a�adido correctamente
    }else
    {
        return 1; // 1 = buffer lleno, imposible a�adir
    }
}


/************************************************ decode_lock() *****
 *
 *   Esta funcion devuelve un char y no necesita de parametros para
 *  funcionar.
 *
 *  Crea la variable "operation" donde se asigna el cambio de estado
 * de la cerradura al final del metodo. Para acometer dicha tarea la
 * funcion lee el estado actual (abierto=LOCK_OPENED, cerrado = LOCK_CLOSED, etc.)
 * mediante la variable lock_state. 
 * 
 *  Est� dise�ada para ser llamada desde el main o la RAI, usa
 * la funci�n 'decode_lock_external_buffer()' para decodificar el c�digo
 * del buffer propio del m�dulo.
 *
 ********************************************************************/
char decode_lock(void)
{
    char operation = LOCK_UNCHANGED;
    
    CloseTimer0(); // Deshabilita la cuenta del timeout
    
    if (current_buffer_position==0) // Buffer vacio
        return operation;

    decode_lock_external_buffer(lock_buffer[0], lock_buffer[1], lock_buffer[2],
            lock_buffer[3], lock_buffer[4], lock_buffer[5]);
    
    empty_lock_buffer();
    return operation;
}

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
        char b4, char b5)
{
    char operation = LOCK_UNCHANGED;

    if (lock_state == LOCK_OPENED)
    {
        if (b0==0x0C && b1==0xFF)
        {
            lock_state = LOCK_CLOSED;
            operation = LOCK_CLOSING;
        }
        else if (b0==0x0B && b5==0xFF)
        {
            lock_code[0] = b1;
            lock_code[1] = b2;
            lock_code[2] = b3;
            lock_code[3] = b4;

            operation = LOCK_PASSWORD_CHANGED;
        }
    }else // if (lock_state == LOCK_CLOSED)
    {
        if (b0==0x0A)
        {
            if(b1==lock_code[0] && b2==lock_code[1] &&
                    b3==lock_code[2] && b4==lock_code[3]
                    && b5==0xFF)
            {
                lock_state = LOCK_OPENED;
                operation = LOCK_OPENING;
            }
            else
            {
                operation = LOCK_WRONG_CLOSING_CODE_ENTERED;
            }
        }
    }
    return operation;
}


/****************************************** empty_lock_buffer() *****
 *
 *   Funci�n que vacia el buffer de entrada a base de reescribir los valores
 * contenidos en �l por el valor por defecto (0xFF).
 *
 ********************************************************************/
void empty_lock_buffer(void)
{
    lock_buffer[0] = 0xFF;
    lock_buffer[1] = 0xFF;
    lock_buffer[2] = 0xFF;
    lock_buffer[3] = 0xFF;
    lock_buffer[4] = 0xFF;
    lock_buffer[5] = 0xFF;
}


/********************************** start_lock_buffer_timeout() *****
 *
 *   Funci�n que inicia el timeout del buffer (est� funcion se usar� tras
 * haber pulsado una tecla).
 *
 ********************************************************************/
void start_lock_buffer_timeout(char f_osc)
{
    INTCONbits.GIEH = 1;
    OpenTimer0(TIMER_INT_ON && T0_16BIT && T0_SOURCE_INT && T0_EDGE_RISE && 
            T0_PS_1_256);

    WriteTimer0 (0xFFFF-(LOCK_TIMEOUT/(f_osc/4)));
    INTCONbits.TMR0IF = 0;
}


/********************************** check_lock_buffer_timeout() *****
 *
 *   Funci�n que chequea si el TMR0 ha saltado activando el flag, de ser
 * as� devuelve 1 y resetea el flag, en caso contrario devuelve 0.
 *
 ********************************************************************/
unsigned char check_lock_buffer_timeout(void)
{
    if (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;
        return 1;
    }
        return 0;
}

/********************************************* get_lock_state() *****
 *
 *   Funci�n que devuelve el valor de la variable lock_state.
 *
 ********************************************************************/
char get_lock_state(void)
{
    return lock_state;
}
