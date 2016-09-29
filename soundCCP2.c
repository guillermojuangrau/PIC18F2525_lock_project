/***** soundCCP2.c **************************************************
 *                                                                  *
 * Biblioteca:                                                      *
 *                                                                  *
 * Autor: Antonio Ángel Pastor Valles                               *
 *        Guillermo de Juan Grau                                    *
 * Fecha: abril de 2012                                             *
 *                                                                  *
 ********************************************************************/
#include "soundCCP2.h"
#include "delays_ms.h"

/*
 * DEFINICIÓN DE CONSTANTES DE USO INTERNO
 */

#define MELODY1_N_NOTES 68
#define MELODY2_N_NOTES 46

#define SILENCE 0.00

/* Notas de la octava 5 */
#define DO_5    523.25
#define DOS_5   554.37
#define RE_5    587.33
#define RES_5   622.25
#define MI_5    659.26
#define FA_5    698.46
#define FAS_5   739.99
#define SOL_5   783.99
#define SOLS_5  830.61
#define LA_5    880.00
#define LAS_5   932.33
#define SI_5    987.77

/* Notas de la octava 6 */
#define DO_6    1046.50
#define DOS_6   1108.73
#define RE_6    1174.66
#define RES_6   1244.51
#define MI_6    1318.51
#define FA_6    1396.91
#define FAS_6   1479.98
#define SOL_6   1567.98
#define SOLS_6  1661.22
#define LA_6    1760.00
#define LAS_6   1864.66
#define SI_6    1975.53

/* Notas de la octava 7 */
#define DO_7    2093.00
#define DOS_7   2217.46
#define RE_7    2349.32
#define RES_7   2489.02
#define MI_7    2637.02
#define FA_7    2793.83
#define FAS_7   2959.96
#define SOL_7   3135.96
#define SOLS_7  3322.44
#define LA_7    3520.00
#define LAS_7   3729.31
#define SI_7    3951.07


/*
 * PROTOTIPOS FUNCIONES INTERNAS
 */
void play_tone_5ms (float frec, unsigned char duration, char f_osc);


/*
 * DECLARACIÓN DE VARIABLES INTERNAS
 */

/* Frecuencia de las notas de la melodía 1 (0 = silencio) */
const rom float melody1_notes [MELODY1_N_NOTES]={ SILENCE, MI_6, SILENCE, MI_6,
                        SILENCE, MI_6, SILENCE, DO_6, MI_6, SOL_6, SOL_5,
                        DO_6, SOL_5, SILENCE, MI_5, SILENCE, LA_5, SI_5, SILENCE,
                        LAS_5, LA_5, SOL_5, MI_6, SOL_6, LA_6,
                        FA_6, SOL_6, SILENCE, MI_6, DO_6, RE_6, SI_5,
                        DO_6, SILENCE, SOL_5, SILENCE, MI_5, SILENCE, LA_5, SI_5,
                        SILENCE, LAS_5, LA_5, SOL_5, MI_6, SOL_6, LA_6,
                        FA_6, SOL_6, SILENCE, MI_6, DO_6, RE_6, SI_5,
                        SILENCE, SOL_6, FAS_6, FA_6, RES_6, MI_6, SILENCE,
                        SOLS_5, LA_5, DO_6, SILENCE, LA_5, DO_6, RE_6};

/* Duración de las notas de la melodía 1 en multiplos de 5 ms */
const rom unsigned char melody1_durations [MELODY1_N_NOTES]={ 32, 3, 1, 7, 1, 4,
                        4, 4, 8, 16, 16, 12, 4, 8, 8, 4, 8, 4, 4, 4, 8, 8, 4, 4,
                        8, 4, 4, 4, 8, 4, 4, 12, 4, 8, 4, 8, 8, 4, 8, 4, 4, 4,
                        8, 8, 4, 4, 8, 4, 4, 4, 8, 4, 4, 16, 4, 4, 4, 4, 8, 4,
                        4, 4, 4, 4, 4, 4, 4, 4};

/* Frecuencia de las notas de la melodía 2 (0 = silencio) */
const rom float melody2_notes [MELODY2_N_NOTES]= {SI_5, LA_5, SOLS_5, LA_5, DO_6,
                        SILENCE, RE_6, DO_6, SI_5, DO_6, MI_6, SILENCE, FA_6,
                        MI_6, RES_6, MI_6, SI_6, LA_6, SOLS_6, LA_6, SI_6, LA_6,
                        SOL_6, LA_6, DO_7, LA_6, DO_7, SOL_6, LA_6, SI_6, LA_6,
                        SOL_6, LA_6, SOL_6, LA_6, SI_6, LA_6, SOL_6, LA_6, SOL_6,
                        LA_6, SI_6, LA_6, SOL_6, FA_6, MI_6};

/* Duración de las notas de la melodía 2 en multiplos de 5 ms */
const rom unsigned char melody2_durations [MELODY2_N_NOTES]= {16, 16, 16, 16,
                        32, 32, 16, 16, 16, 16, 32, 32, 16, 16, 16, 16, 16, 16,
                        16, 16, 16, 16, 16, 16, 64, 32, 32, 4, 4, 32, 32, 32,
                        32, 4, 4, 32, 32, 32, 32, 4, 4, 32, 32, 32, 32, 32};


/************************************************ setup_sound() *****
 *  Función que configura las salidas para poder usar el altavoz como periferico.
 *
 *
 *
 *  Primero comprueba si el CCP2 esta configurado para salir por RB3 o por RC1 y
 * configura dicho puerto como salida. Activa el módulo CCP2 y el flag de dicho módulo
 * así como el TMR3 que será usado junto por dicho módulo.
 *
 ********************************************************************/
void setup_sound (void)
{
    /* configura el pin de CCP2 como salida */
#if CCP2MX == PORTBE
    TRISBbits.TRISB3 = 0; 
#else
    TRISCbits.TRISC1 = 0;
#endif
    CCP2CON = 0x02; // compare mode, toggle output on match (CCPxlF bit is set)
    PIR2bits.CCP2IF = 0;

    /* Habilita timer 3 en modo 16-bit con preescalado 1:8
     *  del ciclo de instrucción */
    T3CON = 0b10111001;
}


/************************************************ disable_sound() *****
 *  Función que deshabilita el sonido.
 *
 *  Para acometer dicha tarea, deshabilita el módulo CCP2, el TM3 y 
 * pone el pin usado por el CCP2 en nivel bajo.
 *
 *
 *
 ********************************************************************/
void disable_sound (void)
{
#if CCP2MX == PORTBE
    PORTBbits.RB3 = 0;
#else
    PORTCbits.RC1 = 0;
#endif
    CCP2CON = 0x00;
    PIR2bits.CCP2IF = 0;
    T3CONbits.TMR3ON = 0;
}


/************************************************** play_beep() *****
 *  Función que reproduce un beep, recibiendo como variable la frecuencia
 * del oscilador.
 *
 *
 ********************************************************************/
void play_beep (char f_osc)
{
    setup_sound();

    play_tone_5ms (DO_6, 100, f_osc);
    wait_5ms(f_osc);

    disable_sound();
}


/********************************************* play_long_beep() *****
 *  Función que reproduce un beep de mayor longitud en el tiempo 
 * que el anterior.
 *
 *
 ********************************************************************/
void play_long_beep (char f_osc)
{
    char reps = 4;
    setup_sound();

    while (reps--)
    {
        play_tone_5ms (DO_6, 100, f_osc);
    }
    wait_5ms(f_osc);
    
    disable_sound();
}


/************************************************ play_melody() *****
 *   Función que recibe como parametros un numero y la frecuencia del
 *  oscilador. Esta función reproduce la melodia nº X almacenada en la
 *  memoria del microprocesador, siendo X el numero pasado como parametro
 * (en este caso solo hay dos melodias almacenadas).
 *
 ********************************************************************/
void play_melody (unsigned char n_melody, char f_osc)
{
    unsigned char n_nota;

    setup_sound();

    switch(n_melody)
    {
        case 1:
            for (n_nota=0; n_nota < MELODY1_N_NOTES; n_nota++)
            {
                play_tone_5ms (melody1_notes [n_nota],
                                    melody1_durations [n_nota], f_osc);
            }
            break;

        case 2:
            for (n_nota=0; n_nota < MELODY2_N_NOTES; n_nota++)
            {
                play_tone_5ms (melody2_notes [n_nota],
                                    melody2_durations [n_nota], f_osc);
            }
            break;
    }

    disable_sound();
}


/********************************************** play_tone_5ms() *****
 *  Función que obtiene como parametros una frecuencia a la que queremos
 * reproducir un tono, un numero que será la cantidad de tonos de duración
 * 5mS que se reproducirán seguidos y la frecuencia del oscilador.
 *
 ********************************************************************/
void play_tone_5ms (float frec, unsigned char duration, char f_osc)
{
    if(frec!=SILENCE)
    {
        float T_tone = 1/frec;
        float T_inst = 4 * (1/(f_osc*1e6));
        unsigned int t_toogle = (T_tone/2) / (T_inst/8); // Timer3 está preescalado a 1:8
        unsigned int t_note = (5e-3 * duration) / (T_tone/2);

        CCPR2 = (TMR3H*0x0100)+TMR3L + t_toogle;

        while(t_note--)
        {
            while (!(PIR2bits.CCP2IF));
            PIR2bits.CCP2IF = 0;
            CCPR2 += t_toogle;
        }
    }else
    {
        while(duration--)
        {
            wait_5ms(f_osc);
        }
    }
}
