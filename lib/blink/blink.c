#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <util/delay.h>

#include "blink.h"

void blink() {
  DDRB |= 0b11111111;    

	uint8_t port;

	while (1) {
		port = 0;
		for (int i = 0; i < 8; i += 1) {
			port |= 1 << i;

			PORTB = port;
			_delay_ms(SLEEP);
		}

		port = 0;
		for (int i = 0; i < 8; i += 1) {
			port = ~port;

			PORTB = port;
			_delay_ms(2*SLEEP);
		}

		port = 0;
		for (int i = 8; i >= 0; i -= 1) {
			port |= 1 << i;

			PORTB = port;
			_delay_ms(SLEEP);
		}
	}
}
