#include <stdint.h>
#include "uart.h"
#include "gpio.h"


int main(){
    // Init knapp A
    GPIO0->PIN_CNF[14] = 1; 

    // Init knapp B
    GPIO0->PIN_CNF[23] = 1;

    // Init UART 
    uart_init();

    // Lys tilstand
    int state = 0;
    
    while(1){
        // Når knapp B trykkes
        if(!((1 << 23) & GPIO0->IN)){
            uart_send('B');
        }

        // Npr knapp A trykkes
        if(!((1 << 14) & GPIO0->IN)){
            uart_send('A');
        }

        if(uart_read() != '\0'){
            if(state){
                gpio_lights_off();
                state--;
            }
            else {
                gpio_lights_on();
                state++;
            }
        }
    }


    return 0;
}