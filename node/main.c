#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "TWI_Slave.h"
#include <string.h>
#include <util/delay.h>

#define CPS 65535

#define TWI_CMD_MASTER_WRITE 0x10
#define TWI_CMD_MASTER_READ  0x20

volatile uint8_t color = 0;
volatile uint8_t light = 0;

ISR(TIMER1_OVF_vect) {

  PORTB &= 0xF0;
  PORTB ^= (1 << light);

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
  PORTB = 0xF0;
  DDRD = 0xFF;

  chain_disable();

  TWI_Slave_Initialise( (unsigned char)((0x00<<TWI_ADR_BITS) | (TRUE<<TWI_GEN_BIT) ));

  sei();

  TWI_Start_Transceiver();

  unsigned char messageBuf[TWI_BUFFER_SIZE];
  uint8_t myaddr = 0, sentack = 0;

  for (;;) {

    if (!TWI_Transceiver_Busy() && TWI_statusReg.lastTransOK) {

      if (!myaddr) {
        if (TWI_statusReg.RxDataInBuf) {

          TWI_Get_Data_From_Transceiver(messageBuf, 1);

          myaddr = (uint8_t)messageBuf[0];
          color = myaddr%3;
          cli();
          TWI_Slave_Initialise( (unsigned char)((myaddr<<TWI_ADR_BITS) | (TRUE<<TWI_GEN_BIT) ));
          sei();

          TWI_Start_Transceiver();
          TCCR1B |= (1 << CS11);
          TIMSK1 |= (1 << TOIE1);
        }
      } else if (!sentack) {
        sentack = 1;
        messageBuf[0] = myaddr;
        TWI_Start_Transceiver_With_Data( messageBuf, 1 );
      } else {
        // TODO: protocol
      }
    }
  }

  return 0;
}