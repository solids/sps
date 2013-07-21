#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "I2C_Slave.h"

#define CPS 65535

volatile uint8_t color = 0;
volatile uint8_t light = 0;

uint8_t leds[4][3] = {
  { 1, 0, 0},
  { 1, 0, 0},
  { 0, 0, 0},
  { 0, 0, 0}
};

volatile uint8_t currentSensor = 0;

// ISR(ADC_vect) {
//   unsigned int result   = ADC;  // Read the ADC Result
//   ADCSRA |=(1 << ADIF);         // Acknowledge the ADC Interrupt Flag

//   if (result > 512) {
//     leds[0][0] = 1024;
//   } else {
//     leds[0][0] = 0;
//   }

// //  currentSensor++;

//   // ADMUX |= (0xf0 | currentSensor);

//   // if (currentSensor > 8) {
//   //   currentSensor = 0;
//   // }
// }

ISR(TIMER1_OVF_vect) {

  PORTB &= 0xF0;
  PORTB |= (1 << light);
  // if (leds[light][0]) {
  //   PORTB |= (1 << light);
  //   PORTD = 0xff ^ _BV(6);
  // }

  // light++;
  // if (light > 3) {
  //   light = 0;
  // }



  // TCNT1 = 61000;

  PORTD |= _BV(3) | _BV(5) | _BV(6);

  switch (color) {
    case 0:
      PORTD &= 0xff ^ _BV(3);
    break;

    case 1:
      PORTD &= 0xff ^ _BV(5);
    break;

    case 2:
      PORTD &= 0xff ^ _BV(6);
    break;
  }



  light++;
  if (light > 3) {
    light = 0;
    color++;
    if (color > 2) {
      color = 0;
    }
  }
}

void chain_disable() {
  // Wait for an address, then turn the next child on.
  PORTD &= 0xFF ^ _BV(2);
}

void chain_enable() {
  PORTD |= _BV(2);
}

int main() {
  DDRB = 0xFF;
  PORTB = 0xFF;
  DDRD = 0xFF;

  chain_disable();

  // ADCSRA   = 0b10101100;  // Enable ADC with Clock prescaled by 16 ; If Clock speed is 8MHz, then ADC clock = 8MHz/16 = 500kHz
  // ADCSRB   = 0b00000000;  // Free Running ADC mode
  // DIDR0    = 0b00111111;  // Disable Digital Input on ADC Channel 0 to reduce power consumption
  // ADMUX    = 0b11000000;  // Disable Left-Adjust and select Internal 1.1V reference and ADC Channel 0 as input

  TCCR1B |= (1 << CS11);
  TIMSK1 |= (1 << TOIE1);
  //TCNT1 = 64000;

  I2C_init(0x00);

  sei();

  // ADCSRA  |= (1<<ADSC);  // Start ADC Conversion

  for (;;) {


  }

  return 0;
}