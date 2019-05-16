#include <SPI.h>
#include <SD.h>

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


// The application object
OPCApp TheApplication (&LightMeter, &DefaultConfigurationData);    
  
void setup(){

  Serial.begin(115200);
  while (!Serial);

  // Initialize the I2C bus
  Wire.begin();
  
  // Uses the default SCL and SDA pins
  LightMeter.begin();

  if (!SD.begin())
    Serial.println ("SD Card not working");
   else
    Serial.println ("SD card seems to work");

  TheApplication.DumpConfig();
  Serial.println ("Loading file number");
  TheApplication.LoadNextFileNumber();

  Serial.println(F("BH1750 Test begin"));

}

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

  TheApplication.CaptureTask();
  
  if (captureWasInProgress && (TheApplication.CaptureInProgress() == false))
  {
     Serial.println ("Capture terminated\n");
     captureWasInProgress = false;
  }

//  unsigned short lux = LightMeter.GetLightIntensity();
//  Serial.print(F("Light: "));
//  Serial.print(lux);
//  Serial.println(F(" lx"));

//  delay(5000);
//  delay(1);
}
