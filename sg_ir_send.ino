//
// taken from IRSend library 
// https://github.com/z3t0/Arduino-IRremote
//
// 
//

#define SONY_HDR_MARK             2400
#define SONY_HDR_SPACE             600
#define SONY_ONE_MARK             1200
#define SONY_ZERO_MARK             600

#define IR_LED_PIN 3

#define SYSCLOCK  16000000  // main Arduino clock
#define TIMER_ENABLE_PWM    (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_PWM   (TCCR2A &= ~(_BV(COM2B1)))
#define TIMER_ENABLE_INTR   (TIMSK2 = _BV(OCIE2A))
#define TIMER_DISABLE_INTR  (TIMSK2 = 0)
#define TIMER_CONFIG_KHZ(val) ({ \
  const uint8_t pwmval = SYSCLOCK / 2000 / (val); \
  TCCR2A               = _BV(WGM20); \
  TCCR2B               = _BV(WGM22) | _BV(CS20); \
  OCR2A                = pwmval; \
  OCR2B                = pwmval / 3; \
})

#define TIMER_PWM_PIN  3
void enableIROut (int khz)
{
  TIMER_DISABLE_INTR; //Timer2 Overflow Interrupt
  pinMode(IR_LED_PIN, OUTPUT);
  digitalWrite(IR_LED_PIN, LOW); // When not sending PWM, we want it low

  // COM2A = 00: disconnect OC2A
  // COM2B = 00: disconnect OC2B; to send signal set to 10: OC2B non-inverted
  // WGM2 = 101: phase-correct PWM with OCRA as top
  // CS2  = 000: no prescaling
  // The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR2A.
  
  TIMER_CONFIG_KHZ(khz);
}

void disableIROut ()
{
 // TIMER_ENABLE_INTR;

  TIMER_DISABLE_PWM;
  pinMode(IR_LED_PIN, INPUT_PULLUP);
  //  analogWrite(pin, 0);
  //  digitalWrite(pin, LOW);
    
}

void sendSony(unsigned long data,  int nbits)
{
  // Set IR carrier frequency
enableIROut(56);

  // Header
  mark(SONY_HDR_MARK);
  space(SONY_HDR_SPACE);

  // Data
  for (unsigned long  mask = 1UL << (nbits - 1);  mask;  mask >>= 1) {
    if (data & mask) {
      mark(SONY_ONE_MARK);
      space(SONY_HDR_SPACE);
    } else {
      mark(SONY_ZERO_MARK);
      space(SONY_HDR_SPACE);
      }
    }

delay(200);
disableIROut();
  // We will have ended with LED off
}


//+=============================================================================
// Sends an IR mark for the specified number of microseconds.
// The mark output is modulated at the PWM frequency.
//
void  mark (unsigned int time)
{
  TIMER_ENABLE_PWM; // Enable pin 3 PWM output
  if (time > 0) custom_delay_usec(time);
}

//+=============================================================================
// Leave pin off for time (given in microseconds)
// Sends an IR space for the specified number of microseconds.
// A space is no output, so the PWM output is disabled.
//
void  space (unsigned int time)
{
  TIMER_DISABLE_PWM; // Disable pin 3 PWM output
  if (time > 0) custom_delay_usec(time);
}


void custom_delay_usec(unsigned long uSecs) {
  if (uSecs > 4) {
    unsigned long start = micros();
    unsigned long endMicros = start + uSecs - 4;
    if (endMicros < start) { // Check if overflow
      while ( micros() > start ) {} // wait until overflow
    }
    while ( micros() < endMicros ) {} // normal wait
  } 
 
}
