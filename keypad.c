/***** keypad.c ****************************************************
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
#include "keypad.h"
#include "delays_ms.h"

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
void setup_keypad (void)
{
    TRISA &= 0xF0; // Clear bits 0-3, set RA3..RA0 como salida
    TRISB |= 0x17; // Set bits 0-2 y 4, set RB4, RB2..RB0 como entrada

}


/**************************************** setup_kp_interrupts() *****
 *  Funci�n para configurar un teclado matricial hexadecimal (4x4)
 *
 *  Para detectar la pulsaci�n de alguna tecla se pretende hacer uso
 * de las interrupciones RB0/INT0, RB1/INT1, RB2/INT2 (interrupciones
 * de flanco) y RB4/KBI0 (interrupci�n de cambio de nivel)
 *
 *  Se configuran todas las interrupciones con un nivel de prioridad
 * alto, ya que INT0 siempre es de prioridad alta para poder gestionar
 * junto a INT1, INT2 y la interrupci�n de cambio en RB4 mediante la
 * funci�n 'check_kp_flags ()'
 *
 *  Necesita que despues de cada lectura de teclado se reconfiguren
 * las filas a nivel alto, para detectar los flancos de subida y el
 * cambio de nivel.
 *
 ********************************************************************/
void setup_kp_interrupts (void)
{
    setup_keypad ();

    INTCONbits.GIEH = 1;
    INTCONbits.INT0IE = 1;
    INTCON3 = 0xD8; // INT1 e INT2 habilitadas con prioridad alta y flags a cero
    INTCON2 |= 0x70; // INT2...INT0 en flanco de subida
    INTCONbits.RBIE = 1;
    INTCON2bits.RBIP = 1;
    INTCONbits.INT0IF = 0;
    INTCONbits.RBIF = 0;
    PORTA |= 0x0F;
}


/************************************* get_key_from_interrupt() *****
 *   Funci�n para lectura de un teclado matricial hexadecimal (4x4)
 * de contactos met�licos, los cuales producen rebotes durante
 * al menos los primeros 5 ms de la pulsaci�n de la tecla.
 *
 *  Dise�ada para ser llamada desde la RAI al ser activada por pulsaci�n del
 * teclado. La funci�n espera 10 ms antes leer el teclado, desechando as� el
 * posible "ruido". La pulsaci�n de una tecla con el dedo dura un minimo de 20 ms.
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
unsigned char get_key_from_interrupt(char f_osc)
{
    unsigned char key;
    
    wait_10ms(f_osc);
    key = get_key();
    PORTA |= 0x0F;

    return key;
}


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
unsigned char get_key(void)
{
/* clear RA0 para escanear la primera fila */
    PORTA |= 0x0F;
    PORTAbits.RA0 = 0;
    if (!(PORTBbits.RB0))
        return 0x01;
    if (!(PORTBbits.RB1))
        return 0x02;
    if (!(PORTBbits.RB2))
        return 0x03;
    if (!(PORTBbits.RB4))
        return 0x0A;

/* clear RA1 para escanear la segunda fila */
    PORTA |= 0x0F;
    PORTAbits.RA1 = 0;
    if (!(PORTBbits.RB0))
    	return 0x04;
    if (!(PORTBbits.RB1))
    	return 0x05;
    if (!(PORTBbits.RB2))
	return 0x06;
    if (!(PORTBbits.RB4))
	return 0x0B;

/* clear RA2 para escanear la tercera fila */
    PORTA |= 0x0F;
    PORTAbits.RA2 = 0;
    if (!(PORTBbits.RB0))
        return 0x07;
    if (!(PORTBbits.RB1))
	return 0x08;
    if (!(PORTBbits.RB2))
	return 0x09;
    if (!(PORTBbits.RB4))
	return 0x0C;

/* clear RA3 para escanear la cuarta fila */
    PORTA |= 0x0F;
    PORTAbits.RA3 = 0;
    if (!(PORTBbits.RB0))
    	return 0x0F;
    if (!(PORTBbits.RB1))
	return 0x00;
    if (!(PORTBbits.RB2))
	return 0x0E;
    if (!(PORTBbits.RB4))
    	return 0x0D;

    return 0xFF;
}

/********************************************* check_kp_flags() *****
 *  Funci�n que comprueba si alguno de los flags que se activan debido
 * a las interrupciones INT0, INT1, INT2 o KBI0 esta activa. Si es
 * as� entonces pone a 0 el flag y activa una variable que ser� devuelta
 * al final de la funci�n.
 *
 *  Cabe destacar que ya que KBI0 es una interrupci�n por cambio de nivel
 * y no por flanco de subida o bajada, tenemos que comprobar adem�s que
 * ha sido provocada por RB4 pasando de nivel alto a nivel bajo y no 
 * viceversa. Es por eso que hay un estamento que comprueba que el pin
 * RB4 est� a nivel bajo y no a nivel alto.
 ********************************************************************/
unsigned char check_kp_flags (void)
{
    char flag = 0;

    if (INTCONbits.INT0IF)
    {
        INTCONbits.INT0IF = 0;
        flag = 1;
    }
    if (INTCON3bits.INT1IF)
    {
        INTCON3bits.INT1IF = 0;
        flag = 1;
    }
    if (INTCON3bits.INT2IF)
    {
        INTCON3bits.INT2IF = 0;
        flag = 1;
    }
    if (INTCONbits.RBIF)
    {
        if(!(PORTBbits.RB4))
            flag = 1;

        INTCONbits.RBIF = 0;
    }
    return flag;
}


/************************************** disable_kp_interrupts() *****
 *  Funci�n que deshabilita las interrupciones usadas durante el programa 
 * mediante los registros INTCON3 e INTCON.
 *
 ********************************************************************/
void disable_kp_interrupts (void)
{
    INTCON3bits.INT2IE = 0;
    INTCON3bits.INT1IE = 0;
    INTCONbits.INT0IE = 0;
    INTCONbits.RBIE = 0;
}


/*************************************** check_kp_int_enabled() *****
 *  Funci�n que devuelve 1 si las interrupciones estan correctamente
 * configuradas y 0 si no lo est�n.
 *
 ********************************************************************/
char check_kp_int_enabled (void)
{
    if (INTCON3bits.INT2IE && INTCON3bits.INT1IE && INTCONbits.INT0IE
            && INTCONbits.RBIE)
        return 1;
    else
        return 0;
}
