#include "gpio.h"
#include <stdio.h>


#define GPIO_BASE_PORT0 0x50000000
#define GPIO_BASE_PORT1 0x50000300

typedef struct {
  volatile uint32_t OUT; 
  volatile uint32_t OUTSET;
  volatile uint32_t OUTCLR;
  volatile uint32_t IN;
  volatile uint32_t DIR;
  volatile uint32_t DIRSET;
  volatile uint32_t DIRCLR;
  volatile uint32_t RESERVED[120];
  volatile uint32_t PIN_CNF[32];
}GPIO_Registers;

GPIO_Registers *gpio_ports[2] = {
  (GPIO_Registers *)(GPIO_BASE_PORT0 + 0x504),
  (GPIO_Registers *)(GPIO_BASE_PORT1 + 0x504)
};


//extern GPIO_Registers * gpio_ports[2];

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
  uint8_t port = gpio_num >> 5;
  uint8_t pin = gpio_num & 0x1F;
  if (port > 1) return;

  if (dir == GPIO_OUTPUT){
    gpio_ports[port]->DIRSET = (1 << pin); // Set as output
  } else {
    gpio_ports[port]->DIRCLR = (1 << pin); // Set as input
  }

  // Configure the pin for the appropriate direction and other settings
  gpio_ports[port]->PIN_CNF[pin] = (dir == GPIO_OUTPUT) ?
    (1 << 0) |   // Configure as output
    (0 << 1) |   // Disconnect input buffer
    (0 << 2) |   // No pull
    (0 << 8) |   // Standard drive strength
    (0 << 16)    // Sense disabled
    :
    (0 << 0) |   // Configure as input
    (1 << 1) |   // Connect input buffer
    (1 << 2) |   // Pull down enabled
    (0 << 8) |   // Standard drive strength
    (0 << 16);   // Sense disabled
}



// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_set(uint8_t gpio_num) {
  uint8_t port = gpio_num >> 5;
  uint8_t pin = gpio_num & 0x1F;
  if (port > 1) return;
  gpio_ports[port]->OUTSET = (1 << pin);
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_clear(uint8_t gpio_num) {
  uint8_t port = gpio_num >> 5;
  uint8_t pin = gpio_num & 0x1F;
  if (port > 1) return;
  gpio_ports[port]->OUTCLR = (1 << pin);
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
// Output:
//  bool - pin state (true == high)
bool gpio_read(uint8_t gpio_num) {
  uint8_t port = gpio_num >> 5;
  uint8_t pin = gpio_num & 0x1F;
  if (port > 1) return false;
  return (gpio_ports[port]->IN & (1 << pin)) != 0;
}

// prints out some information about the GPIO driver. Can be called from main()
void gpio_print(void) {
  printf("OUT register for Port 0: %p\n", &(gpio_ports[0]->OUT));
  printf("DIR register for Port 0: %p\n", &(gpio_ports[0]->DIR));
  printf("PIN_CNF[0] for Port 0: %p\n", &(gpio_ports[0]->PIN_CNF[0]));
  printf("OUT register for Port 1: %p\n", &(gpio_ports[1]->OUT));
  printf("DIR register for Port 1: %p\n", &(gpio_ports[1]->DIR));
  printf("PIN_CNF[0] for Port 1: %p\n", &(gpio_ports[1]->PIN_CNF[0]));

} 







