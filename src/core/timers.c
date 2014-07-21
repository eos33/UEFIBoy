#include "IO.h"
#include "timers.h"
#include "memory.h"
#include "memory_layout.h"
#include "bits.h"

#define DIV_TIMER_INC_FREQUENCY 16382

//Possible timer increment timer_frequencies in hz
#define TIMER_FREQUENCIES_LEN sizeof (timer_frequencies) / sizeof (long)
static const long timer_frequencies[] = {4096, 262144, 65536, 16384}; 

static long clock_speed = GB_CLOCK_SPEED_HZ;

static long timer_frequency = -1;
static long timer_counter = 0;

/*  Set and get the clockspeed in hz */
void set_clock_speed(long hz) {
    clock_speed = hz;
}

long get_clock_speed() {
    return clock_speed;
}

/* Change the timer frequency to another of the possible
 * frequencies, resets the timer_counter 
 * If frequency number selected isn't valid then nothing happens*/
void set_timer_frequency(unsigned int n) {
    if (n < TIMER_FREQUENCIES_LEN) {
        timer_frequency = timer_frequencies[n];
    }
}


void update_divider_reg(long cycles) {
    static long divider_counter = 0;

    divider_counter += cycles;

    // Increment div at a frequency of 16382hz
    long max_counter = clock_speed / DIV_TIMER_INC_FREQUENCY;
    while (divider_counter >= max_counter) {
        increment_div();
        divider_counter -= clock_speed / max_counter;
    }
}


void update_timers(long cycles) { 
    uint8_t timer_control = get_mem(TAC_REG);
    
    update_divider_reg(cycles);
    //Clock enabled
    if ((timer_control & BIT_2) != 0) {
        if (timer_frequency == -1) { // If timer not set
           set_timer_frequency(timer_control & 3);
        }
        timer_counter += cycles;
        /* Once timer incremented, check for changes in timer frequency,
         * and reset timer */
        while (timer_counter >= (clock_speed / timer_frequency)) {
            timer_counter -= (clock_speed / timer_frequency);
            increment_tima();
            set_timer_frequency(timer_control & 3);
        }
    }
}





