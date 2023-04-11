#include "timer1.h"

#define TCCR1A  (*((volatile unsigned char *) 0x80))
#define TCCR1B  (*((volatile unsigned char *) 0x81))
#define TCCR1C  (*((volatile unsigned char *) 0x82))
#define TIMSK1  (*((volatile unsigned char *) 0x6F))
#define OCR1A   (*((volatile unsigned int *) 0x88))
#define SREG    (*((volatile unsigned char *) 0x5F))
#define TCINTH    (*((volatile unsigned char *) 0x85))
#define TCINTL    (*((volatile unsigned char *) 0x84))
#define CPU_FREQUENCY 16000000UL
#define F_CPU 16000000UL

//Defining bits
#define WGM12 3
#define CS12 2
#define CS10 0
#define OCIE1A 1

#define I_SREG 7
static volatile unsigned long count = 0;

void timer1_init() {
    // Set timer1 compare for 1s tick
    // Calculation: (CPU_FREQUENCY / PRESCALER_VALUE / COMPARE_VALUE) = 1 Hz (1s)
    // Example: (16,000,000 / 1024 / 15625) = 1 Hz

    /*
    unsigned int PRESCALER_VALUE = 1024;
    */
    unsigned int COMPARE_VALUE = 15625;  //16 bit max value = 2^16 - 65535

    // Set CTC mode
    TCCR1B |= (1 << WGM12); // WGM12

    // Set clock divisor (prescaler value = 1024)
    TCCR1B |= (1 << CS12) | (1 << CS10); // CS12 and CS10

    // Set compare value
    OCR1A = COMPARE_VALUE - 1;

    // Enable interrupts on output compare A
    TIMSK1 |= (1 << OCIE1A); // OCIE1A

    // Enable global interrupts
    // __builtin_avr_sei();
    SREG |= (1<<I_SREG);
}

unsigned long timer1_get() {
    unsigned long count_value;

    // Get global interrupt enable bit state
    unsigned int sreg_state = SREG & (1 << I_SREG);

    // Disable interrupts
    // __builtin_avr_cli();
    SREG &= ~(1<<I_SREG);

    // Get the count value
    count_value = count;

    // Restore global interrupt state
    if (sreg_state) {
        // __builtin_avr_sei();
        SREG |= (1<<I_SREG);
    }

    // Return the count value
    return count_value;
}

void timer1_clear() {
    // Get global interrupt enable bit state
    unsigned int sreg_state = SREG & (1 << I_SREG);

    // Disable interrupts
    // __builtin_avr_cli();
    SREG &= ~(1<<I_SREG);

    // Clear count value
    count = 0;

    // Restore global interrupt state
    if (sreg_state) {
        // __builtin_avr_sei();
        SREG |= (1<<I_SREG);
    }
}


void __vector_11(void) __attribute__ ((signal, used, externally_visible));

void __vector_11(void)
{
    // Increment the count by 1
    count++;
}

/*
ISR(TIMER1_COMPA_vect){
    __vector_11();
}

*/
