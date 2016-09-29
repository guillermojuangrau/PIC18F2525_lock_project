/***** lock4_usart.h ******************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio Ángel Pastor Valles                               *
 *                                                                  *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/
#ifndef _LOCK4_USART_
#define _LOCK4_USART_

#define LOCK_USART_TIMEOUT 60 // En segundos

/************************************************ setup_usart() *****
 *
 *
 *
 *
 ********************************************************************/
void setup_lock_usart (int f_osc);

/***************************************** disable_lock_usart() *****
 *
 *
 *
 *
 ********************************************************************/
void disable_lock_usart (void);

/************************************** add2lock_usart_buffer() *****
 *
 *
 *  !!!! Recibimos carácteres ascii¿?!!!!!!!!!!!!!!!!!! TRANSFORMAR!!!
 *
 ********************************************************************/
char add2lock_usart_buffer(char f_osc);


/****************************************** decode_lock_usart() *****
 *
 *
 *
 *
 ********************************************************************/
char decode_lock_usart(void);

/****************************************** decode_lock_usart() *****
 *
 *
 *
 *
 ********************************************************************/
char ascii2lock_raw (char ascii);

/************************************ empty_lock_usart_buffer() *****
 *
 *
 *
 *
 ********************************************************************/
void empty_lock_usart_buffer(void);


/********************************** start_lock_usart_buffer_timeout() *****
 *
 *
 *
 *
 ********************************************************************/
void start_lock_usart_buffer_timeout(char f_osc);


/**************************** check_lock_usart_buffer_timeout() *****
 *
 *
 ********************************************************************/
unsigned char check_lock_usart_buffer_timeout(void);

#endif
