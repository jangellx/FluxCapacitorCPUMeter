/*
  Flux Capacitor CPU Meter
 
 Reads from the serial port, flashing the LEDs blue based on CPU activity.
 Changes the LEDs to red in a low-memory condition.
 */

int redLEDs[3]  = { 5, 9, 11 };
int blueLEDs[3] = { 3, 6, 10 };

unsigned long sinceFadeStarted[3] = {0, 0, 0};              // Time when the fade started on the bright LEDs
unsigned long fadeDuration        = 1000;                   // Current fade duration in milliseconds, as computed from CPU time
int           brightLEDStep       =    0;                   // Index of the currently bright LED step
int           doRedLEDs           =    0;                   // True12 if we should light the red LEDs (low memory) instead of the blue LEDs

#define       FADE_OFFSET         (fadeDuration * 0.6)      // Delay between each LED step

// updateFadeStartTimes()
//  Updates the sinceFadeStarted() array based on the current step's start
//  time.  Most useful when the fade duration has changed.
void updateFadeStartTimes( void ) {
  int  prevStep = brightLEDStep;
  int  curStep  = prevStep - 1;
  
  for( int i=0; i < 2; i++ ) {
    if( curStep < 0 )
      curStep = 2;

    sinceFadeStarted[ curStep ] = sinceFadeStarted[ prevStep ] - FADE_OFFSET;

    prevStep = curStep--;
  }
}

// setup()
void setup() {
  // Initialize our PWM LED pins as outputs
  for( int i=0; i < 3; i++ ) {
    pinMode( redLEDs[i],  OUTPUT );
    pinMode( blueLEDs[i], OUTPUT );

    digitalWrite( redLEDs[  i ], 1 );                 // Turn off the red LEDs
    digitalWrite( blueLEDs[ i ], 0 );                 // Turn off the blue/green LEDs
  }

  // Set up the serial port
  Serial.begin( 9600 );

  // Initialize the fader
  sinceFadeStarted[0] = millis();
  updateFadeStartTimes();
}

// lightLEDs()
//  Sets the LED intensity via PWM.  0 turns them all the way off, and 1 turns them
//  all the way on.  The brightStep determines which of the three sets of LEDs we're
//  adjusting (the rest will be set to off), and doRedsOnly determines if we're
//   lighitng up as red or white.
void lightLEDs( int doRedsOnly, int brightStep, int pwm ) {
  if( doRedsOnly ) {
    // Red LEDs only
    analogWrite(  redLEDs[ brightStep ], pwm );                // Set the level on the red LEDs
    digitalWrite( blueLEDs[ brightStep ], LOW );               // Turn off the blue/green LEDs

  } else {
    // All LEDs for a white light
    //  The blue/green pins are connected through an N-channel transistor, so the PWM is inverted
    //  for them vs. the red pins.  Also, the red is much brighter, so we need to dim it.
    int  redPWM = constrain( (255 * .6666) + (pwm * .3333) + 1, 0, 255 );    // +1 is to make sure we hit 255 to turn off the LEDs
    analogWrite(  redLEDs[ brightStep ], redPWM );            // Need to dim it due to how bright the red is
    analogWrite( blueLEDs[ brightStep ], 255 - pwm );         // Need to invert due to transistor
  }
}

// fadeLEDs()
//  Compute the amount of fade for a given step from the current time.
void fadeLEDs( unsigned long now, int curStep ) {
  float          elapsed;                             // Elapsed time since the fade for brightLEDStep started
  int            pwm;                                 // PWM for the LED step
  float          progress, curved, scaled;

  // Compute the fade of the LEDs based on how long it's been since we started fading
  elapsed = now - sinceFadeStarted[ curStep ];

  if( (elapsed < 0) || (elapsed >= fadeDuration) ) {
    pwm = 255;    // 255 is full off (inverted due to common-cathode LEDs)
  } else {
    // Fade up quickly, then fade down slowly
    if( elapsed > fadeDuration * 0.3 ) {
      elapsed = fadeDuration - elapsed;
      progress = ((float)elapsed) / (((float)fadeDuration) * 0.7);
    } else {
      progress = ((float)elapsed) / (((float)fadeDuration) * 0.3);
    }

    curved = cos( progress * PI/2.0 );
    scaled = curved * 255.0;
    pwm    = constrain( (int)scaled, 0, 255 );
  }

  // Light the appropriate LEDs
  lightLEDs( doRedLEDs, curStep, pwm );
}

// Check for input on the serial port
//  Data is a single byte.  The high bit determines if we're showing the red LEDs (true) or
//  the blue (false).  The remaining bits are the fade duration, which ranges from 100 to
//  1100 ms.  We read all waiting data from the serial port, but use only the last byte.
void serialEvent() {
  char  c;
  
  while( Serial.available() )
       c = Serial.read();

  doRedLEDs = c & 0x80;
  Serial.print( "c was hex " );
  Serial.print( c, HEX );
  Serial.print( ", dec " );
  Serial.println( c, DEC );

  char  durPart = c & ~0x80;              // Strip off the high bit (red on/off)
  float percent = ((float)(durPart)) / 127.0;    // Convert to a percentage

  fadeDuration = (unsigned long)(percent * 1000.0) + 100;  // Range is 100-1100 ms
  updateFadeStartTimes();

  Serial.print( "fadeDuration now " );
  Serial.println( fadeDuration );
  Serial.print( "Red LEDs are now " );
  Serial.println( doRedLEDs ? "on" : "off" );
}

void loop() {
  unsigned long  now = millis();

  for( int i=0; i < 3; i++ ) {
    fadeLEDs( now, i );
  }

  if( (now - sinceFadeStarted[ brightLEDStep ]) >= FADE_OFFSET ) {
    // Finished fading; move on to the next LED step
    if( ++brightLEDStep > 2 )
      brightLEDStep = 0;

    sinceFadeStarted[ brightLEDStep ] = millis();
    updateFadeStartTimes();
  }
}

