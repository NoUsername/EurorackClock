#pragma once

// debug mode
#define DEBUG 0

#define ANALOG_OFFSET_USED 0
#define ANALOG_OFFSET_MIN 460
#define ANALOG_OFFSET_MAX 905

// number of BPM when turned all of the way to the left
#define MIN_BPM 40

// number of BPM when turned all of the way to the right
#define MAX_BPM 300

// bias number of MS for turning off of the signal
#define BIAS 10

// potentiometer inputs
#define BPM      A2
#define DURATION A1

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
