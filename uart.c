#include <stdint.h>
#include "uart.h"
#include "gpio.h"


#define UART ((NRF_UART_REG*)0x40002000) //gpio 0 base adress

typedef struct{
    volatile uint32_t TASKS_STARTRX;
    volatile uint32_t TASKS_STOPRX;
    volatile uint32_t TASKS_STARTTTX;
    volatile uint32_t TASKS_STOPTX;
    volatile uint32_t RESERVED0[3];
    volatile uint32_t TASKS_SUSPEND;
    volatile uint32_t RESERVED1[56];
    volatile uint32_t CTS;
    volatile uint32_t NCTS;
    volatile uint32_t RXDREADY;
    volatile uint32_t RESERVED2[4];
    volatile uint32_t TXDREADY;
    volatile uint32_t RESERVED3;
    volatile uint32_t ERROR;
    volatile uint32_t RESERVED4[7];
    volatile uint32_t RXTO;
    volatile uint32_t RESERVED5[110];
    volatile uint32_t INTEN;
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t RESERVED6[93];
    volatile uint32_t ERRORSRC;
    volatile uint32_t RESERVED7[31];
    volatile uint32_t ENABLE;
    volatile uint32_t RESERVED8;
    volatile uint32_t PSELRTS;
    volatile uint32_t PSELTXD;
    volatile uint32_t PSELCTS;
    volatile uint32_t PSELRXD;
    volatile uint32_t RXD;
    volatile uint32_t TXD;
    volatile uint32_t RESERVED9;
    volatile uint32_t BAUDRATE;
    volatile uint32_t RESERVED10[17];
    volatile uint32_t CONFIG;
}NRF_UART_REG;


void uart_init(){
    // Konfigurerer pinnene
    // RX
    GPIO0->PIN_CNF[6] = 0;
    // TX
    GPIO1->PIN_CNF[8] = 1;

    UART->PSELTXD = 0b000110;
    UART->PSELRXD = 0b101000;

    // Setter baudrate
    UART->BAUDRATE = 0x00275000;

    // Definerer at vi ikke har CTS- eller PTS-koblinger
    UART->PSELRTS = 1;
    UART->PSELCTS = 1;

    // Skrur på UART-modulen
    UART->ENABLE = 4;
    UART->TASKS_STARTRX = 1;
}


void uart_send(char letter){
    UART->TASKS_STARTTTX = 1;
    UART->TXD = letter;
    while(!(UART->TXDREADY)){

    }
    UART->TXDREADY = 0;
    UART->TASKS_STOPTX = 1;
}


char uart_read(){
    UART->TASKS_STARTRX = 1;

    if(!(UART->RXDREADY)){
        return '\0';
    }
    else {
        UART->RXDREADY = 0;
        return UART->RXD;
    }
}


