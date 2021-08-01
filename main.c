/*
 * File:   main.c
 * Author: marcos-bah
 *
 * Created on July 20, 2021, 9:42 AM
 */

#include<time.h> //necessário p/ função time()
#include <stdlib.h>// necessário p/ as funções rand() e srand()

#include "keypad.h"
#include "lcd.h"
#include "bits.h"
#include "config.h"
#include "ssd.h"
#include "io.h"
#include "pwm.h"
#include <pic18f4520.h>
#include "apostas.h"

#define ON 0x0F
#define L0 0x80
#define L1 0xC0
#define CLR 0x01



void main(void) {
    unsigned int tecla = 16;
    float tempo;
    //configuração do sistema
    srand(time(NULL));
    pwmInit();
    ssdInit();
    lcdInit();
    kpInit();
    lcdCommand(ON);
    textInit();
    textSaldo();
    textInstrutions();
    textWait();
    for (;;) {
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();
            if (bitTst(tecla, 7)) { //2
                lcdCommand(CLR);
                textApostando();
                textWait();
            }
            if (bitTst(tecla, 3)) { //1
                lcdCommand(CLR);
                textSaldo();
                textApostas();
                textWait();

            }
            if (bitTst(tecla, 8)) { //#
                lcdCommand(CLR);
            }

        }

    }
}
