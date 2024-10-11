// Interrupt app
//
// Trigger GPIO and Software interrupts

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"

#include "microbit_v2.h"

void gpiote_init(void){
  NRF_GPIOTE->CONFIG[0] = (1 << GPIOTE_CONFIG_MODE_Pos)
                          | (14 << GPIOTE_CONFIG_PSEL_Pos)
                          | (0 << GPIOTE_CONFIG_PORT_Pos)
                          | (2 << GPIOTE_CONFIG_POLARITY_Pos);

  NRF_GPIOTE->INTENSET = (1 << GPIOTE_INTENSET_IN0_Pos);

  NVIC_EnableIRQ(GPIOTE_IRQn);
  NVIC_SetPriority(GPIOTE_IRQn, 2);                        
}

// Initialize software interrupts
void software_interrupt_init(void) {
  NRF_EGU1->INTENSET = (1 << EGU_INTENSET_TRIGGERED0_Pos);
  NVIC_EnableIRQ(SWI1_EGU1_IRQn);
  NVIC_SetPriority(SWI1_EGU1_IRQn, 4);
}

// Trigger a software interrupt
void software_interrupt_trigger(void) {
  NRF_EGU1->TASKS_TRIGGER[0] = 1;
}

void SWI1_EGU1_IRQHandler(void) {
  // Clear interrupt event
  NRF_EGU1->EVENTS_TRIGGERED[0] = 0;

  printf("Software interrupt running... \n");
  for (int i = 0; i < 5; i++){
    printf("Processing step %d\n", i + 1);
    nrf_delay_ms(1000);
  }
  printf("Software interrupt completed!\n");

}

void GPIOTE_IRQHandler(void) {
  // Clear interrupt event
  if(NRF_GPIOTE->EVENTS_IN[0]) {
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    printf("Button A pressed! Preempting the software interrupt.\n");
    software_interrupt_trigger();

  }
}

int main(void) {
  printf("Board started!\n");

  // First task. Trigger a GPIOTE interrupt
  // You can access the GPIOTE register map as NRF_GPIOTE->
  //    where the register name in all caps goes after the arrow.
  //    For example, NRF_GPIOTE->CONFIG[0]
  
  gpiote_init();

  software_interrupt_init();

  software_interrupt_trigger();

  // Second task. Trigger a software interrupt
  // Use the software_interupt_* functions defined above
  // Add code here


  // loop forever
  while (1) {
    printf("Looping\n");
    nrf_delay_ms(1000);
  }
}

