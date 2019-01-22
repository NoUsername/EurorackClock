#pragma once

// debug mode
#define DEBUG 0

#define ANALOG_OFFSET_USED 0
#define ANALOG_OFFSET_MIN 460
#define ANALOG_OFFSET_MAX 905

#define MAX_PERCENTAGE 95

// number of BPM when turned all of the way to the left
#define MIN_BPM 30

// number of BPM when turned all of the way to the right
#define MAX_BPM 450

// bias number of MS for turning off of the signal
#define BIAS 2

// potentiometer inputs
#define BPM      A2
#define DURATION A3
// other inputs
#define ROT_DIVIDER          A4
#define SW_DURATION_PER_NOTE A5
#define SW_ODDS_RETRIGGER    A6

// outputs
#define SIXTEENTH 4
#define WHOLE     5
#define HALF      6
#define QUARTER   7
#define EIGHTH    8

#define SIX_THIRDS   9
#define SIX_FIFTHS  10
#define SIX_SEVENTH 11


// screen
#define HAS_SCREEN false
#define SCREEN_ADDR 0x7A



