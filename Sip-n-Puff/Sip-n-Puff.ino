/*
 * Sample Sip and Puff code
 *
 * Modified MPRLS simpletest.ino example
 */

/****************************************************************************
 * This example demostrates how to turn an actuator on and off via a Digital
 * I/O pin. Puff to turn on, Sip to turn off.
 *
 * There's a safety lockout that engages if the actuator is on for more than
 * MAX_TIME_ON_S seconds. You have to reset the micro to clear the lockout.
 *
 * You may want to set a timer to turn off the actuator. Or not - I don't
 * know what the application is.
 ****************************************************************************/

// include library code
#include <Wire.h>

// set to ADAFRUIT or SPARKFUN (or anything else really)
#define SENSOR_VENDOR SPARKFUN

#if SENSOR_VENDOR == ADAFRUIT
  #include <Adafruit_MPRLS.h>
  #define HPA_CONVERSION 1
  #define UNITS
#else
  #include <SparkFun_MicroPressure.h>
  #define HPA_CONVERSION 10.0
  #define UNITS KPA
#endif

// You dont *need* a reset and EOC pin for most uses, so we set to -1 and don't connect
#define RESET_PIN  -1  // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN    -1  // set to any GPIO pin to read end-of-conversion by pin

// this creates an object ("mpr") that embodies the code used to read the pressure sensor
#if SENSOR_VENDOR == ADAFRUIT
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);
#else
SparkFun_MicroPressure mpr = SparkFun_MicroPressure();
#endif

/* Set the pin you want to use to connect to the actuator switching device.
 * With a normal micro you can just use LED_BUILTIN to test with the built-in LED.
 * The QT Py uses a Neopixel multi-colored LED that needs special code
 * that I can provide if desired.
 */
#define ACTUATOR_OUT_PIN 0

// set some pressure thresholds: +/-5% - you'll probably want to change these
#define LOW_PRES_MULT 0.95
#define HI_PRES_MULT  1.05

float average_air_pressure_hPa; // this will be set automatically or you could set it here (use hPA!, not PSI)
float pressure_hPa;             // holds the pressure readings

// track the state of the actuator
bool actuator_on = false;

// safety lockout stuff
#define MAX_TIME_ON_S   25      // max ON time in seconds
uint32_t lockout_time;          // lockout at this time (millis())
bool LOCKOUT_ON = false;        // safety lockout in effect


// setup() runs once before everything else
 
void setup() {
  // set the pin direction and level
  pinMode(ACTUATOR_OUT_PIN, OUTPUT);
  digitalWrite(ACTUATOR_OUT_PIN, LOW);

  // Start the console. Any speed will work the same since it's USB.
  Serial.begin(9600);
  while (not Serial); // we'll wait forever for you to open the Serial Consiole window from the Arduino IDE

  // Connect to the sensor
  Serial.println("MPRLS Simple Test");  // print a line out to the console
  while (not mpr.begin()) { // begin() "initializes" or starts up the sensor
    Serial.println("Failed to communicate with MPRLS sensor, check wiring?");
    delay(30e3); // wait 30 seconds (30,000 ms) and try again
  }

  // all set
  Serial.println("MPRLS sensor READY!");
  set_avg_pressure(); // set the baseline pressure if not assigned above
}


/* loop() runs over and over once setup() has run.
 * Don't put it in a hard loop (e.g., while(1){...})
 *  - let it return to do some system housekeeping chores
 *    like downloading new code.
 */

void loop() {
  // see if safety lockout is in effect
  if (LOCKOUT_ON) { // locked out - hang out for 10s the return ... ... ...
    delay (10e3);
    return;
  }
  // see if we've timed out
  if (actuator_on and (millis() > lockout_time)) {
    turn_actuator_off();
    LOCKOUT_ON = true;
    Serial.println("ERROR: ON time exceeded - SAFETY LOCKOUT in effect.");
    Serial.println("       Reset to restore operation.");
    return;
  }
  // normal operation
  pressure_hPa = mpr.readPressure(UNITS) * HPA_CONVERSION;  // read the pressure
  if (pressure_hPa > average_air_pressure_hPa * HI_PRES_MULT) {
    turn_actuator_on();
  } else if (pressure_hPa < average_air_pressure_hPa * LOW_PRES_MULT) {
    turn_actuator_off();
  }
}

void turn_actuator_on(void) {
  Serial.println("Turning actuator ON!");
  delay(500); // wait 1/2 sec
  digitalWrite(ACTUATOR_OUT_PIN, HIGH);
  actuator_on = true;
  lockout_time = millis() + MAX_TIME_ON_S * 1000;
}

void turn_actuator_off(void) {
  digitalWrite(ACTUATOR_OUT_PIN, LOW);
  actuator_on = false;
  Serial.println("Actuator off");
}

/*
 * Read the pressure 5 times to get an average.
 * Uses the default units of hPA, not PSI as displayed.
 */
void set_avg_pressure (void) {
  // prompt the user to release pressure in tube
  Serial.println("Reading baseline pressure in 5 seconds - release pressure tube.");
  delay(5e3); // wait 5 secs
  int num_reads = 5;
  float pressure_sum = 0;
  for (int i=0 ; i<num_reads ; i++) {
    pressure_hPa = mpr.readPressure(UNITS) * HPA_CONVERSION;  // read the pressure
    pressure_sum += pressure_hPa; // sum the readings to be averaged later
    Serial.print("Pressure (PSI): "); Serial.println(pressure_hPa / 68.947572932);
    delay(1e3); // loop in 1 sec
  }
  average_air_pressure_hPa = pressure_sum/num_reads;  // set average
  Serial.println("Done initializing. Ready for use.");
}
