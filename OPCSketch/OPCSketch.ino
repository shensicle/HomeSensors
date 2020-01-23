
#include <SD.h>
#include <SPI.h>
//#include <EEPROM.h>

//Include the Arduino Stepper Library
#include <Stepper.h>
 
// Light sensor library
#include <BH1750FVI.h>


#include "OPCConfig.h"
#include "OPCApp.h"

/*
  Example of BH1750 library usage.
  This example initialises the BH1750 object using the default high resolution
  one shot mode and then makes a light level reading every five seconds.
  After the measurement the MTreg value is changed according to the result:
  lux > 40000 ==> MTreg =  32
  lux < 40000 ==> MTreg =  69  (default)
  lux <    10 ==> MTreg = 138
  Remember to test your specific sensor! Maybe the MTreg value range from 32
  up to 254 is not applicable to your unit.
  Connection:
    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)
    SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)
    ADD -> (not connected) or GND
  ADD pin is used to set sensor I2C address. If it has voltage greater or equal to
  0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
  0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address will
  be 0x23 (by default).
*/

#include <Wire.h>

// Chip select for SD Card
//#define SD_CS 53


// Object to manage the photodiode
BH1750FVI  LightMeter(BH1750FVI::k_DevModeContLowRes);

// Default configuration to be used if configuration stored in EEPROM is missing or corrupted

static camera_config_t DefaultConfigurationData =
{
  OPC_CONFIGURATION_VERSION,
  DEFAULT_IMAGE_WIDTH,
  DEFAULT_IMAGE_HEIGHT,
  DEFAULT_BIT_DEPTH,
  DEFAULT_COLOUR_MODE,
  0,                     // unused field
  DEFAULT_FILTER_METHOD,
  DEFAULT_SCAN_TYPE
};

// Button stuff
#define BUTTON_PIN 3  // must be a pin that can generate an interrupt

// Stepper motor stuff
 
// Number of steps per internal motor revolution 
const float STEPS_PER_REV = 64; 
 
//  Amount of Gear Reduction
const float GEAR_RED = 64;  
 
// Number of steps per geared output rotation
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;
 
// Number of Steps Required
int StepsRequired;

// Stepper HorizStepper(STEPS_PER_REV, 22, 24, 23, 25);
Stepper HorizStepper(STEPS_PER_REV, 30, 32, 31, 33);
Stepper VertStepper(STEPS_PER_REV, 34, 36, 35, 37);


// The application object
OPCApp TheApplication (&LightMeter, &DefaultConfigurationData);

// ----------------------------------------------------------------------
// Interrupt handler invoked when button is pressed
void ButtonPress()
{

}

// ----------------------------------------------------------------------
void setup() {

  Serial.begin(115200);
  while (!Serial);

  Serial.println ("===============================");
//  Serial.print ("Size is " ); Serial.println (sizeof(camera_config_t));

  // Initialize the I2C bus
  Wire.begin();

  // Initialize SPI bus
  // Shouldn't need this   SPI.begin(18,19,23);

  // Uses the default SCL and SDA pins
  LightMeter.begin();

  if (SD.begin() == true)
   Serial.println ("SD card seems to work");
  else
     Serial.println ("SD Card not working");

  
    TheApplication.DumpConfig();
    Serial.println ("Loading file number");
    TheApplication.LoadNextFileNumber();
  
    // Set up the push button
    pinMode (BUTTON_PIN, INPUT);
    digitalWrite (BUTTON_PIN, HIGH);  // enable internal pullup resistor
    attachInterrupt (digitalPinToInterrupt(BUTTON_PIN), ButtonPress, FALLING);

  Serial.println(F("BH1750 Test begin"));

  // Rotate CCW 1/2 turn quickly
  StepsRequired  = 2048;   // looks like 1024 is 1/2 revolution
  HorizStepper.setSpeed(512);
  VertStepper.setSpeed(512);

}

// ----------------------------------------------------------------------
void loop()
{
  static bool captureWasInProgress = false;

  
    if (Serial.available())
    {
      // Get the new character
      char inChar = toupper((char)Serial.read());

      if (inChar == 'S')
      {
          // Start a capture
          TheApplication.StartNewCapture();
          captureWasInProgress = true;
          Serial.println ("Capture started");
      }

      else if (inChar == 'A')
      {
        TheApplication.AbortCapture();
        Serial.println ("Capture aborted");
      }
    }

    // TheApplication.CaptureTask();

    if (captureWasInProgress && (TheApplication.CaptureInProgress() == false))
    {
       Serial.println ("Capture terminated\n");
       captureWasInProgress = false;
    }
  
  unsigned short lux = LightMeter.GetLightIntensity();
  Serial.print(F("Light: "));
  //  Serial.print ("Size is " ); Serial.println (sizeof(camera_config_t));

  Serial.print(lux);
  Serial.println(F(" lx"));

  HorizStepper.step(StepsRequired);
  //delay(2000);
  VertStepper.step(StepsRequired);

  //delay(2000);
}
