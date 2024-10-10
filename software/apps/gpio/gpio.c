#include "gpio.h"
#include <stdio.h>

typedef struct{
	volatile uint32_t OUT;
    volatile uint32_t OUTSET;
    volatile uint32_t OUTCLR;
    volatile uint32_t IN;
    volatile uint32_t DIR;
    volatile uint32_t DIRSET;
    volatile uint32_t DIRCLR;
    volatile uint32_t LATCH;
    volatile uint32_t DETECTMODE;
    uint32_t _unused_A[(0x700-0x524) / 4 - 1];
    volatile uint32_t PIN_CNF[32];
} gpio_reg_t;

volatile gpio_reg_t* GPIO_REG_ZERO = (gpio_reg_t*) (0x50000000);
volatile gpio_reg_t* GPIO_REG_ONE = (gpio_reg_t*) (0x50000300);

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
  // Implement me
  // This function should configure the pin as an input/output
  // Hint: Use proper PIN_CNF instead of DIR
  uint8_t port_num = gpio_num >> 5;  // port_num is gpio_num / 32 (either 0 or 1)
  uint8_t pin_num = gpio_num & 0x1F; // pin_num is gpio_num % 32 (0 to 31)

  if (dir == GPIO_INPUT) {
  	if (port_num == 0) {
  		GPIO_REG_ZERO->PIN_CNF[pin_num] = 0;
  	} else if (port_num == 1) {
  		GPIO_REG_ONE->PIN_CNF[pin_num] = 3;
  	  }
  } else if (dir == GPIO_OUTPUT) {
  	if (port_num == 0) {
  		GPIO_REG_ZERO->PIN_CNF[pin_num] = 0;
  	} else if (port_num == 1) {
  		GPIO_REG_ONE->PIN_CNF[pin_num] = 3;
  	  }
  	}
  }

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_set(uint8_t gpio_num) {
  // Implement me
  // This function should make the pin high
  // It can assume that the pin has already been configured
  uint8_t port_num = gpio_num >> 5;
  uint8_t pin_num = gpio_num & 0x1F;
  if (port_num == 0) {
  	GPIO_REG_ZERO->PIN_CNF[pin_num] = 0;
  } else if (port_num == 1) {
  	GPIO_REG_ONE->PIN_CNF[pin_num] = 3;
  }
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_clear(uint8_t gpio_num) {
  // Implement me
  // This function should make the pin low
  // It can assume that the pin has already been configured
  uint8_t port_num = gpio_num >> 5;
  uint8_t pin_num = gpio_num & 0x1F;
  if (port_num == 0) {
  	GPIO_REG_ZERO->OUTCLR = (1 << pin_num);
  } else if (port_num == 1) {
  	GPIO_REG_ONE->OUTCLR = (1 << pin_num);
  }
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
// Output:
//  bool - pin state (true == high)
bool gpio_read(uint8_t gpio_num) {
  // Implement me
  // This function should read the value from the pin
  // It can assume that the pin has already been configured
  uint8_t port_num = gpio_num >> 5;
  uint8_t pin_num = gpio_num & 0x1F;
  if (port_num == 0) {
  	return (GPIO_REG_ZERO->IN & (1 << pin_num)) != 0;  // Return true if the pin is high, false if low
  } else if (port_num == 1) {
  	return (GPIO_REG_ONE->IN & (1 << pin_num)) != 0;  // Return true if the pin is high, false if low
  }
  return false;
}

// prints out some information about the GPIO driver. Can be called from main()
void gpio_print(void) {
  // Use this function for debugging purposes
  // For example, you could print out struct field addresses
  // You don't otherwise have to write anything here
  printf("GPIO_REG_ZERO->DIR: %p\n", &(GPIO_REG_ZERO->DIR));
  printf("GPIO_REG_ZERO->OUT: %p\n", &(GPIO_REG_ZERO->OUT));
  printf("GPIO_REG_ZERO->OUTCLR: %p\n", &(GPIO_REG_ZERO->OUTCLR));
  printf("GPIO_REG_ONE->DIR: %p\n", &(GPIO_REG_ONE->DIR));
  printf("GPIO_REG_ONE->OUT: %p\n", &(GPIO_REG_ONE->OUT));
  printf("GPIO_REG_ONE->OUTCLR: %p\n", &(GPIO_REG_ONE->OUTCLR));
}

