/*
 * File:   apostas.c
 * Author: marcos-bah
 *
 * Created on July 21, 2021, 9:40 AM
 */


#include "apostas.h"

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

#define ON 0x0F
#define L0 0x80
#define L1 0xC0
#define CLR 0x01

#define DISP0 PIN_A2
#define DISP1 PIN_A3
#define DISP2 PIN_A4
#define DISP3 PIN_A5

char i;

void escreve(char text[]) {
    char i = 0;
    while (text[i] != '\0') {
        lcdData(text[i]);
        i++;
    }
}

void delay() {
    char i, j;
    for (i = 0; i < 255; i++) {
        for (j = 0; j < 255; j++);
        for (j = 0; j < 255; j++);
    }
}

void verso() {
    delay();
    unsigned char k, x;
    x = 0b10000001;
    for (k = 0; k < 8; k++) {
        delay();
        PORTD = x;
        x = x << 1;
    }
}

void inverso() {
    delay();
    unsigned char k, x;
    x = 0b10000000;
    for (k = 0; k < 8; k++) {
        delay();
        PORTD = x;
        x = x >> 1;
    }
}

void textInit() {
    char text[12] = "Bem Vindo ao";
    char text2[16] = "CASSINO UNIFEI!!!";
    lcdCommand(L0 | (0));
    escreve(text);
    lcdCommand(L1 | (0));
    escreve(text2);
    //delay
    delay();
    delay();
    delay();
    //limpa a tela
    lcdCommand(CLR);
}

char* itoa(int value, char* result, int basea) {
    // check that the base if valid
    if (basea < 2 || basea > 36) {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= basea;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * basea)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void textInstrutions() {
    char text[15] = "Clique em 1";
    char text2[15] = "para o saldo";
    lcdCommand(L0 | (0));
    escreve(text);
    lcdCommand(L1 | (0));
    escreve(text2);
    //delay
    delay();
    delay();
    delay();

    //limpa a tela
    lcdCommand(CLR);
}

void textApostas() {
    char text[15] = "Clique em 2";
    char text2[15] = "para APOSTAR!";
    lcdCommand(L0 | (0));
    escreve(text);
    lcdCommand(L1 | (0));
    escreve(text2);
    //delay
    delay();
    delay();
    delay();
    //limpa a tela
    lcdCommand(CLR);
}

int saldo = 650;

void textSaldo() {
    //limpa a tela
    lcdCommand(CLR);
    char text[6] = "Saldo: ";
    char text2[10] = "";
    itoa(saldo, text2, 10);
    lcdCommand(L0 | (0));
    escreve(text);
    lcdCommand(L1 | (0));
    escreve(text2);
    //delay
    delay();
    delay();
    delay();
    //limpa a tela
    lcdCommand(CLR);
}

void textWait() {
    lcdCommand(CLR);
    char text[15] = "Esperando";
    char text2[15] = "Aposte com 2";
    lcdCommand(L0 | (0));
    escreve(text);
    lcdCommand(L1 | (0));
    escreve(text2);
    //delay
    delay();
    delay();
    delay();

}

void textGameOver() {
    char text[15] = "GAME OVER";
    char text2[20] = "saldo insuf...";
    lcdCommand(L0 | (0));
    escreve(text);
    lcdCommand(L1 | (0));
    escreve(text2);
    //delay
    delay();
    delay();
    delay();
    lcdCommand(CLR);
    textSaldo();
}

void textLost() {
    char text[15] = "VC PERDEU";
    char text2[15] = "Custo: -200 ";
    lcdCommand(L0 | (0));
    escreve(text);
    lcdCommand(L1 | (0));
    escreve(text2);
    //delay
    delay();
    delay();
    delay();
    lcdCommand(CLR);
    textSaldo();
}

void textWin() {
    char text[15] = "VC GANHOU";
    char text2[15] = "Custo: +200 ";
    lcdCommand(L0 | (0));
    escreve(text);
    lcdCommand(L1 | (0));
    escreve(text2);
    //delay
    delay();
    delay();
    delay();
    lcdCommand(CLR);
    saldo += 400; //descontando os 200 ja tirados
    textSaldo();
}

int sorte = 0;

void Apostando() {
    sorte = rand() % 10000;
    sorte -= 1;
    char ssorte[4];
    itoa(sorte, ssorte, 10);
    for (float t = 0; t < 200; t++) {
        ssdDigit(sorte / 1000, 0);
        ssdDigit((sorte / 100) % 10, 1);
        ssdDigit((sorte / 10) % 10, 2);
        ssdDigit(sorte % 10, 3);
        ssdUpdate();
        for (float tempo = 0; tempo < 100; tempo++); // +- 10ms
    }

    digitalWrite(DISP0, 0);
    digitalWrite(DISP1, 0);
    digitalWrite(DISP2, 0);
    digitalWrite(DISP3, 0);
    if (ssorte[0] == ssorte[1] && ssorte[2] == ssorte[3] && ssorte[1] == ssorte[2]) {
        textWin();
    } else {
        textLost();
    }
}

void textApostando() {
    if (saldo - 200 <= 0) {
        textGameOver();
    } else {
        char text[15] = "Apostando";
        char text2[15] = "Custo: -200 ";
        lcdCommand(L0 | (0));
        escreve(text);
        lcdCommand(L1 | (0));
        escreve(text2);
        //aposta
        saldo -= 200;
        pwmSet(99);
        pwmFrequency(1000);
        TRISD = 0x00;
        PORTD = 0x00;
        verso();
        inverso();
        pwmSet(0);
        pwmFrequency(0);
        Apostando();

        lcdCommand(CLR);
    }
}
