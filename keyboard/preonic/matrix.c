/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "backlight.h"

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);


inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // To use PORTF disable JTAG with writing JTD bit twice within four cycles.
    MCUCR |= (1<<JTD);
    MCUCR |= (1<<JTD);

    // Initialize PWM timer for backlight.
    backlight_init_ports();
    
    // initialize row and col
    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
    

    // Do a beep with the speaker.
    
    // Setup PC7 as output and output low.  
    DDRC |= (1<<6);
    PORTC &= ~(1<<6);
    
    // Pin PC6 = OCR3C (Timer 3, Channel C)
    // Compare Output Mode = Clear on compare match, Channel A = COM3A1=1 COM3A0=0
    // (i.e. start high, go low when counter matches.)
    // WGM Mode 14 (Fast PWM) = WGM33=1 WGM32=1 WGM31=1 WGM30=0
    // Clock Select = clk/1 (no prescaler) = CS32=0 CS31=0 CS30=1
    
    TCCR3A = _BV(COM3A1) | _BV(WGM31);
    TCCR3B = _BV(WGM33) | _BV(WGM32) | _BV(CS30);

    int freq = (((double)F_CPU) / 1000);
    ICR3 = freq;
    OCR3A = freq>>2;   
  
    for ( int i=0; i<3; i++ )
    {
        TCCR3A |= _BV(COM3A1);     
        _delay_ms(100);
        TCCR3A &= ~(_BV(COM3A1));
        _delay_ms(100);   
    }    
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}

/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7   8   9   10  11
 * pin: F1  F0  B0  C7  F4  F5  F6  F7  D4  D6  B4  D7
 */
static void  init_cols(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRB  &= ~(1<<0 | 1<<4);
    PORTB |=  (1<<0 | 1<<4);
    DDRC  &= ~(1<<7);
    PORTC |=  (1<<7);
    DDRD  &= ~(1<<4 | 1<<6 | 1<<7);
    PORTD |=  (1<<4 | 1<<6 | 1<<7);
    DDRF  &= ~(1<<0 | 1<<1 | 1<<4 | 1<<5 | 1<<6 | 1<<7);
    PORTF |=  (1<<0 | 1<<1 | 1<<4 | 1<<5 | 1<<6 | 1<<7);
}

static matrix_row_t read_cols(void)
{
    return (PINF&(1<<1) ? 0 : (1<<0)) |
           (PINF&(1<<0) ? 0 : (1<<1)) |
           (PINB&(1<<0) ? 0 : (1<<2)) |
           (PINC&(1<<7) ? 0 : (1<<3)) |
           (PINF&(1<<4) ? 0 : (1<<4)) |
           (PINF&(1<<5) ? 0 : (1<<5)) |
           (PINF&(1<<6) ? 0 : (1<<6)) |
           (PINF&(1<<7) ? 0 : (1<<7)) |
           (PIND&(1<<4) ? 0 : (1<<8)) |
           (PIND&(1<<6) ? 0 : (1<<9)) |
           (PINB&(1<<4) ? 0 : (1<<10)) |
           (PIND&(1<<7) ? 0 : (1<<11));
}

/* Row pin configuration
 * row: 0   1   2   3   4
 * pin: D2  D5  B5  B6  D3
 */
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRB &= ~(1<<5 | 1<<6);
    PORTB |= (1<<5 | 1<<6);
    DDRD &= ~(1<<2 | 1<<3 | 1<<5);
    PORTD |= (1<<2 | 1<<3 | 1<<5);
}

static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            DDRD  |= (1<<2);
            PORTD &= ~(1<<2);
            break;
        case 1:
            DDRD  |= (1<<5);
            PORTD &= ~(1<<5);
            break;
        case 2:
            DDRB  |= (1<<5);
            PORTB &= ~(1<<5);
            break;
        case 3:
            DDRB  |= (1<<6);
            PORTB &= ~(1<<6);
            break;
        case 4:
            DDRD  |= (1<<3);
            PORTD &= ~(1<<3);
            break;
    }
}
