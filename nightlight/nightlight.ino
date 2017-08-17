// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Set constants.
// Set LED pins.
const int LED_RED             = 13;
const int LED_YELLOW          = 12;
const int LED_GREEN           = 8;
const int DELAY_TIME          = 10000;

// Set RTC power pins.
const char RTC_GROUND         = A2;
const char RTC_POWER          = A3;

// Set variables.
// Set standard output.
char output_red               = LOW;
char output_yellow            = LOW;
char output_green             = LOW;

// Set previous ouput.
char previous_output_red      = output_red;
char previous_output_yellow   = output_yellow;
char previous_output_green    = output_green;

// Set timing/time variables.
int query_datetime            = 1;
int rtc_query_timeout         = 86400;
unsigned long previous_millis = 0;
DateTime now;

void setup () {
  
  // Setup pins for output.
  pinMode( RTC_GROUND, OUTPUT );
  pinMode( RTC_POWER,  OUTPUT );
  pinMode( LED_RED,    OUTPUT );
  pinMode( LED_YELLOW, OUTPUT );
  pinMode( LED_GREEN,  OUTPUT );

  // Setup pins for +/- This is a little hack specific for RTC module connected on these pins.
  digitalWrite( A2, LOW   );
  digitalWrite( A3, HIGH  );
  
  while ( !Serial ); // for Leonardo/Micro/Zero

  Serial.begin( 57600 );
  
  if ( ! rtc.begin() ) {
    Serial.println( "Couldn't find RTC" );
    while ( 1 );
  }

  if ( ! rtc.isrunning() ) {
    Serial.println( "RTC is NOT running!" );
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust( DateTime( F(__DATE__), F(__TIME__) ) );
  }

}

void loop () {
  
  // Query RTC for a new datetime if var is set.
  if ( query_datetime == 1 ) {
    now = rtc.now();
    query_datetime = 0;
  }

  // Determine what kind of millis we do need to add to time. Could be current or current - previous.
//  if ( ! previous_millis ) {
//    now.AddMilliseconds( current_millis );
//  }
//  else {
//    unsigned long millis_to_add = current_millis - previous_millis;
//    now.AddMilliseconds( millis_to_add );
//  }

  // Check time and determine which LED(s) should be tunred on/off (HIGH/LOW).
  if ( ( now.hour() == 18 ) && ( now.minute() >= 30 ) ) {
    output_red    = LOW;
    output_yellow = HIGH;
    output_green  = LOW;
  }
  else if ( ( now.hour() >= 19 ) && ( now.hour() <= 24 ) ) {
    output_red    = HIGH;
    output_yellow = LOW;
    output_green  = LOW;
  }
  else if ( now.hour() < 6 ) {
    output_red    = HIGH;
    output_yellow = LOW;
    output_green  = LOW;
  }
  else if ( ( now.hour() == 6 ) && ( now.minute() <= 55 ) ) {
    output_red    = HIGH;
    output_yellow = LOW;
    output_green  = LOW;
  }
  else if ( ( now.hour() == 6 ) && ( now.minute() >= 55 ) ) {
    output_red    = LOW;
    output_yellow = HIGH;
    output_green  = LOW;
  }
  else if ( now.hour() == 7 ) {
    output_red    = LOW;
    output_yellow = LOW;
    output_green  = HIGH;
  }
  else {
    output_red    = LOW;
    output_yellow = LOW;
    output_green  = LOW;
  }

  // Only change led outputs if they've changed.
  if ( ( previous_output_red != output_red ) || ( previous_output_yellow != output_yellow ) || ( previous_output_green != output_green ) ) {
    
    digitalWrite( LED_RED, 	output_red	);
    digitalWrite( LED_YELLOW,	output_yellow	);
    digitalWrite( LED_GREEN, 	output_green	);

    // Save state.
    previous_output_red     = output_red;
    previous_output_yellow  = output_yellow;
    previous_output_green   = output_green;
  }

  query_datetime = 1;
}
