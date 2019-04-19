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
        DEFAULT_HORIZ_RES,
        DEFAULT_VERT_RES,
        DEFAULT_CAPTURE_MODE,
        DEFAULT_BIT_DEPTH
    };


// The application object
OPCApp TheApplication (&LightMeter, &DefaultConfigurationData);    
  
void setup(){

  Serial.begin(115200);

  // Initialize the I2C bus
  Wire.begin();
  
  // Uses the default SCL and SDA pins
  LightMeter.begin();

  TheApplication.DumpConfig();

  Serial.println(F("BH1750 Test begin"));

}

void loop() 
{
  //we use here the maxWait option due fail save
  unsigned short lux = LightMeter.GetLightIntensity();
  Serial.print(F("Light: "));
  Serial.print(lux);
  Serial.println(F(" lx"));

  delay(5000);
}
