#include "SimpleTimer.h"

#include "calc.h"
#include "display.h"
#include "config.h"


// current state
int count = 0;
int thirds = 0;
int fifths = 0;
int seventh = 0;
bool thirdsTriggered = false;
bool fifthsTriggered = false;
bool seventhsTriggered = false;

// settings changeable via knobs/switches
bool durationPerNote = true;
/**
 * what frequency the odds should count 
 */
int incrementOddsEvery = 1; // 1, 2, 4, 8, 16
/**
 * should odd numbers turn on/off with the 16th notes
 */
bool turnOffOddsSixteenth = false;


// timer
SimpleTimer timer;

int analogMultiRead(int port) {
  int total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(port);
  }

  return (int) total / 10;
}

int readRotary(int port) {
  int value = analogRead(port);
  // 6 position rotary switch with 1k resistors in between
  return value / 160;
}

int readSwitch(int port) {
  int value = analogRead(port);
  // 3 state switches with 2 10k resistors in between
  return value / 320;
}

/**
WHOLE = /8
HALF = /4
QUARTER = /2
EIGTH = /1
*/

void cycleOff16() {
  if (!durationPerNote) {
    digitalWrite(WHOLE, LOW);
  	digitalWrite(HALF, LOW);
  	digitalWrite(QUARTER, LOW);
  	digitalWrite(EIGHTH, LOW);
  }
  digitalWrite(SIXTEENTH, LOW);

  if (turnOffOddsSixteenth) {
    digitalWrite(SIX_THIRDS, LOW);
    digitalWrite(SIX_FIFTHS, LOW);
    digitalWrite(SIX_SEVENTH, LOW);
  } else if (count % incrementOddsEvery == 0) {
    digitalWrite(SIX_THIRDS, LOW);
    digitalWrite(SIX_FIFTHS, LOW);
    digitalWrite(SIX_SEVENTH, LOW);
  }

  count++;

  if (count == 16) {
    count = 0;
  }

  if (count % incrementOddsEvery == 0) {
    thirds++;
    fifths++;
    seventh++;
  }

  if (thirds == 3) {
    thirds = 0;
    thirdsTriggered = false;
  }
  if (fifths == 5) {
    fifths = 0;
    fifthsTriggered = false;
  }
  if (seventh == 7) {
    seventh = 0;
    seventhsTriggered = false;
  }
}

void cycleOff8() {
	digitalWrite(WHOLE, LOW);
}

void cycleOff4() {
	digitalWrite(HALF, LOW);
}

void cycleOff2() {
	digitalWrite(QUARTER, LOW);
}

void cycleOff1() {
	digitalWrite(EIGHTH, LOW);
}

/*
 1. * * * * *
 2. *
 3. * *
 4. *
 5. * * *
 6. *
 7. * *
 8. *
 9. * * * *
10. *
11. * *
12. *
13. * * *
14. *
15. * *
16. *
 */

void cycleOn() {
  switch (count) {
    case 0:
      digitalWrite(WHOLE, HIGH);
      digitalWrite(HALF, HIGH);
      digitalWrite(QUARTER, HIGH);
      digitalWrite(EIGHTH, HIGH);
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 1:
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 2:
      digitalWrite(WHOLE, HIGH);
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 3:
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 4:
      digitalWrite(WHOLE, HIGH);
      digitalWrite(HALF, HIGH);
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 5:
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 6:
      digitalWrite(WHOLE, HIGH);
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 7:
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 8:
      digitalWrite(WHOLE, HIGH);
      digitalWrite(HALF, HIGH);
      digitalWrite(QUARTER, HIGH);
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 9:
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 10:
      digitalWrite(WHOLE, HIGH);
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 11:
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 12:
      digitalWrite(WHOLE, HIGH);
      digitalWrite(HALF, HIGH);
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 13:
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 14:
      digitalWrite(WHOLE, HIGH);
      digitalWrite(SIXTEENTH, HIGH);
      break;

    case 15:
      digitalWrite(SIXTEENTH, HIGH);
      break;
  }

  if (thirds == 0) {
    if (turnOffOddsSixteenth || !thirdsTriggered) {
      digitalWrite(SIX_THIRDS, HIGH);
    }
    thirdsTriggered = true;
  }
  
  if (fifths == 0) {
    if (turnOffOddsSixteenth || !fifthsTriggered) {
      digitalWrite(SIX_FIFTHS, HIGH);
    }
    fifthsTriggered = true;
  }

  if (seventh == 0) {
    if (turnOffOddsSixteenth || !seventhsTriggered) {
      digitalWrite(SIX_SEVENTH, HIGH);
    }
    seventhsTriggered = true;
  }

  // figure out bpm and duration values
  int bpm_pot = analogMultiRead(BPM);
  int duration_pot = analogMultiRead(DURATION);
  int bpm = round(calculatedBPM(bpm_pot));
  float percentage = calculatedPercentage(duration_pot);
  float originalPercentage = percentage;
  
  // at high bpms, we need to have the best chance possible to get a clean signal across
  //   so the other side detects it correctly
  if (bpm > 130) {
    percentage = 0.5;
  }
  unsigned long duration = tickDuration(bpm * 4);

  // figure out the next cycle off and set it based on bpm
  unsigned long next_start = duration;

  // figure out the next cycle on and set it based on bpm
  unsigned long next_stop = (unsigned long) (duration * percentage);
  // For these times it's ok to calc with originalPercentage since these signals
  //  will never get 'that' fast.
  unsigned long next_stop_8 = (unsigned long) (duration *  2 * originalPercentage);
  unsigned long next_stop_4 = (unsigned long) (duration *  4 * originalPercentage);
  unsigned long next_stop_2 = (unsigned long) (duration *  8 * originalPercentage);
  unsigned long next_stop_1 = (unsigned long) (duration * 16 * originalPercentage);

  int dividerSetting = readRotary(ROT_DIVIDER);
  if (dividerSetting == 0) {
    incrementOddsEvery = 1;
  } else if (dividerSetting == 1) {
    incrementOddsEvery = 2;
  } else if (dividerSetting == 2) {
    incrementOddsEvery = 4;
  } else if (dividerSetting == 3) {
    incrementOddsEvery = 8;
  } else { // note pos 5 & 6 are the same
    incrementOddsEvery = 16;
  }
  int durationPerNoteSetting = readSwitch(SW_DURATION_PER_NOTE);
  durationPerNote = durationPerNoteSetting == 0;
  int oddsRetriggerSetting = readSwitch(SW_ODDS_RETRIGGER);
  turnOffOddsSixteenth = oddsRetriggerSetting == 0;

  // set the timers
  timer.setTimeout(next_start, cycleOn);
  timer.setTimeout(next_stop, cycleOff16);
  if (durationPerNote && (count % 2 == 0)) {
  	timer.setTimeout(next_stop_8, cycleOff8);
  }
  if (durationPerNote && (count % 4 == 0)) {
  	timer.setTimeout(next_stop_4, cycleOff4);
  }
  if (durationPerNote && (count % 8 == 0)) {
  	timer.setTimeout(next_stop_2, cycleOff2);
  }
  if (durationPerNote && (count == 0)) {
  	timer.setTimeout(next_stop_1, cycleOff1);
  }

#if HAS_SCREEN
  display_bpm(bpm);
#endif

#if DEBUG
Serial.print("analog value bpm: ");
  Serial.println(bpm_pot);
  Serial.print("analog value duration: ");
  Serial.println(duration_pot);
  Serial.print("Calculated BPM: ");
  Serial.println(bpm);
  Serial.print("Percentage: ");
  Serial.println(percentage);
  Serial.print("Start: ");
  Serial.println(next_start);
  Serial.print("Stop:  ");
  Serial.println(next_stop);
  Serial.print("Count: ");
  Serial.println(count);
  Serial.print("DurationPerNoteSetting: ");
  Serial.println(durationPerNoteSetting);
  Serial.print("OddsRetriggerSetting: ");
  Serial.println(oddsRetriggerSetting);
#endif
}

void setup() {
#if DEBUG
  Serial.begin(115200);
  Serial.println("Setup");
#endif

  pinMode(BPM, INPUT);
  pinMode(DURATION, INPUT);
  pinMode(ROT_DIVIDER, INPUT);
  pinMode(SW_DURATION_PER_NOTE, INPUT);
  pinMode(SW_ODDS_RETRIGGER, INPUT);
  
  pinMode(WHOLE, OUTPUT);
  pinMode(HALF, OUTPUT);
  pinMode(QUARTER, OUTPUT);
  pinMode(EIGHTH, OUTPUT);
  pinMode(SIXTEENTH, OUTPUT);
  
  pinMode(SIX_THIRDS, OUTPUT);
  pinMode(SIX_FIFTHS, OUTPUT);
  pinMode(SIX_SEVENTH, OUTPUT);

#if HAS_SCREEN
  setup_display();
#endif

#if DEBUG
  int val = analogMultiRead(BPM);
  Serial.print("BPM Read: ");
  Serial.println(val);
  Serial.print("Calculated BPM: ");
  Serial.println(round(calculatedBPM(val)));
#endif
}

bool started = false;

void loop() {
  // run the first on cycle, which should set up the first set of timers
  if (!started) {
    cycleOn();
    started = true;
  }

  // turn on the timer
  timer.run();
}



