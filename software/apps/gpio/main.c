// GPIO app
//
// Uses MMIO to control GPIO pins

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"

#include "microbit_v2.h"
#include "gpio.h"

#define ROW1 21
#define ROW2 22
#define ROW3 15
#define ROW4 24
#define ROW5 19
#define COL1 28
#define COL2 11
#define COL3 31
#define COL4 37
#define COL5 30

#define GPIO_LED 20
#define GPIO_BUTTON_A 14
#define GPIO_BUTTON_B 23


void configure_leds(void){
  uint8_t rows[] = {ROW1, ROW2, ROW3, ROW4, ROW5};
  uint8_t cols[] = {COL1, COL2, COL3, COL4, COL5};

  // Configure rows as output and set high (turn off LEDs)
  for(int i = 0; i < 5; i++){
    gpio_config(rows[i], GPIO_OUTPUT);
    gpio_set(rows[i]);
  }

  // Configure columns as output and set high initially
  for(int i = 0; i < 5; i++){
    gpio_config(cols[i], GPIO_OUTPUT);
    gpio_set(cols[i]); // Set columns high initially
  }
}


void display_pattern(void){
  uint8_t rows[] = {ROW1, ROW2, ROW3, ROW4, ROW5};
  uint8_t cols[] = {COL1, COL2, COL3, COL4, COL5};

  for(int r = 0; r < 5; r++){
    // Activate the current row by setting it low
    gpio_set(rows[r]);
  }
  for(int c = 0; c < 5; c++){
    // Activate the current row by setting it low
    gpio_clear(cols[c]);
  }
}

void configure_buttons(void) {
    // Configure Button A as input with pull-down resistor
    NRF_P0->PIN_CNF[GPIO_BUTTON_A] = (0 << 0) | // Configure as input
                                     (1 << 1) | // Connect input buffer
                                     (1 << 2) | // Enable pull-down resistor
                                     (0 << 8) | // Standard drive strength
                                     (0 << 16); // Sense disabled

    // Configure Button B as input with pull-down resistor
    NRF_P0->PIN_CNF[GPIO_BUTTON_B] = (0 << 0) | // Configure as input
                                     (1 << 1) | // Connect input buffer
                                     (1 << 2) | // Enable pull-down resistor
                                     (0 << 8) | // Standard drive strength
                                     (0 << 16); // Sense disabled
}

int main(void) {
  printf("Board started!\n");

  configure_leds();
  configure_buttons();
  gpio_print();

  // Control LED with raw MMIO
  // Microphone LED is P0.20 and active high
  gpio_config(GPIO_LED, GPIO_OUTPUT);

  //gpio_config(GPIO_BUTTON_A, GPIO_INPUT);

  //gpio_config(GPIO_BUTTON_B, GPIO_INPUT);
  
  
  
  // loop forever
  printf("Looping\n");
  while (1) {
      configure_buttons();

      bool buttonA_pressed = !(NRF_P0->IN & (1 << GPIO_BUTTON_A));
      bool buttonB_pressed = !(NRF_P0->IN & (1 << GPIO_BUTTON_B));

      if (buttonA_pressed) {
          printf("Button A is pressed\n");
      }

      if (buttonB_pressed) {
          printf("Button B is pressed\n");
      }
    
      display_pattern();
    // Control LED with buttons
    // Button A is P0.14 and active low
    // Button B is P0.23 and active low
    // Add code here
    nrf_delay_ms(100);
  }
} 



