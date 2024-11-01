#include <ESP8266WiFi.h>
#include "IFTTTMessage.h"

#include "WaterSensor.h"
#include "TemperatureSensor.h"
#include "CheckIn.h"
#include "SensorConfigDefs.h"
#include "SensorConfig.h"
#include "SensorSerialInterface.h"
#include "SimpleLED.h"

#define FIRMWARE_VERSION "V1.3"

// Constants we use in the loop() method to serivce hardware at different intervals
unsigned int DelayPerLoop         = 50;  // milliseconds - has to be fast enough for terminal interface
const unsigned int WaterSensorUpdateInterval = 10000;  // check water sensor every 10 seconds
const unsigned int TemperatureSensorUpdateInterval  = 10000;  // check temperature sensor every 10 seconds

// Object to manage the onboard LED
SimpleLED TheLED (LED_BUILTIN, false, DelayPerLoop, 500); // FLash LED every 500 ms

// Flag which, when set, indicates that a device startup notification has been sent to ifttt.com. An attempt
// is made to send at startup, but if power to the wifi router was also interrupted, it will not likely be
// operational before this device is up and running.
bool StartupMessageSent = false;

// Flag which, when set, indicates that our configuration is valid. If the configuration is not valid
// only the terminal interface is serviced so that the user can set the configuration.
bool ConfigurationIsValid = true;
// --------------------------------------------------------------------------------------------------

IFTTTMessageClass IFTTTSender;   // Communicates with ifttt.com

// Object that stores our configuration
SensorConfigClass TheConfiguration;

// Water sensor stuff
const int  WaterSensorPin  = A0;        // Analog IO pin connected to water level sensor
WaterSensorClass TheWaterSensor (WaterSensorPin, 
                                 DelayPerLoop, 
                                 WaterSensorUpdateInterval, 
                                 &IFTTTSender,
                                 &TheConfiguration);

// Temperature/Humidity sensor stuff
const int  TemperatureSensorPin  = 4;        // Digital IO pin connected sensor
TemperatureSensorClass TheTemperatureSensor (TemperatureSensorPin, 
                                 DelayPerLoop, 
                                 TemperatureSensorUpdateInterval, 
                                 &IFTTTSender,
                                 &TheConfiguration); 

// Daily check in
const unsigned long defaultCheckInInterval = DEFAULT_CHECK_IN_MINUTES * 60 * 1000;
CheckInClass TheCheckIn (DelayPerLoop, 
                         defaultCheckInInterval, 
                         &IFTTTSender,
                         &TheConfiguration,
                         &TheTemperatureSensor,
                         &TheWaterSensor);

// Buzzer stuff
//const int TheBuzzerPin = D4;          // Digital IO pin for buzzer

// Object that manages the terminal interface
SensorSerialInterface TheSerialInterface(&TheConfiguration);


// --------------------------------------------------------------------------------------------------
void setup() {

  // Start serial communication for debug information
  Serial.begin(115200); 

  Serial.print (F("\n\n\nshensicle sensor platform - Firmware version "));
  Serial.print (FIRMWARE_VERSION);
  Serial.println (F(" - type 'H' for help\n"));
  
  // Set up LED to flash so user knows we're alive
  TheLED.SetFlashMode(true);

  // Read the configuration from EEPROM
  bool okay = TheConfiguration.Load();
  
  // If the personality checksum test passed
  if (okay == true)
  {
    Serial.print ("Sensor ID: "); Serial.println (TheConfiguration.GetUUID());

    // We can now initialize fields to be sent to IFTTT that were in the personality
    IFTTTSender.Initialize (TheConfiguration.GetIFTTTKey(), TheConfiguration.GetUUID());


   // And if we have a buzzer, set it up too
// if (TheConfiguration.HasBuzzer())
//     pinMode(TheBuzzerPin, OUTPUT);

    // Now connect to Wifi for the first time and send a startup message
    ConnectWifi(TheConfiguration.GetWifiSSID(), TheConfiguration.GetWifiPassword()); 

    // If wifi connected, send device startup message
    if (WiFi.status() == WL_CONNECTED)
    {
        IFTTTSender.Send ("Device startup");
        StartupMessageSent = true;           // don't want to do this again until next power up
    }

    // Now that the configuration is loaded, we can update interval for check in
    TheCheckIn.SetUpdateInterval((unsigned long)TheConfiguration.GetCheckInMinutes()*60*1000);
  }

  else
  {
      ConfigurationIsValid = false;
      
      // Speed up LED flashing to alert the user
      TheLED.SetUpdateInterval(100);
      
      Serial.println (F("*** Configuration checksum failure ***"));
      Serial.println (F("Use the 'C' and 'S' commands to configure this unit\n"));
  }
}

// --------------------------------------------------------------------------------------------------
void loop()
{  
  
  // Check the serial interface for a complete command and, if there is one, execute it
  TheSerialInterface.Update();

  // If our configuration is not valid, we can't do much except wait for one on the terminal interface
  if (ConfigurationIsValid == true)
  {
      // If  we're not connected to Wifi ...
      if (WiFi.status() != WL_CONNECTED) 
      {
          ConnectWifi(TheConfiguration.GetWifiSSID(), TheConfiguration.GetWifiPassword());  // Connect to WiFi

          // If we are now connected and this is the first successful connect, send a device startup message. This handles the
          // case where power to the whole house goes off and this device comes up before the Wifi does
          if ((WiFi.status() == WL_CONNECTED) && (StartupMessageSent == false))
          {
              StartupMessageSent = true;
              IFTTTSender.Send ("Device startup (delayed waiting for Wifi)");
          }

      }
      else  // We are connected to Wifi. Read sensors and deal with the result
      {

        if (TheConfiguration.HasWaterSensor())
          TheWaterSensor.Service();

        if (TheConfiguration.HasTemperatureSensor())
          TheTemperatureSensor.Service();

         if (TheConfiguration.CheckIn())
          TheCheckIn.Service();
      }
  }
  
  // Service serial port - it's supposed to happen automatially but that doesn't
  // seem to work on the NodeMCU
  SerialEvent();

  // Service the LED
  TheLED.Service();
  
  // Give up to background processing
   delay (DelayPerLoop);
}

// --------------------------------------------------------------------------------------------------
// Tries to connect to the wireless access point with the credentials provided.  The idea is to call
// this multiple times, until connection is established, so that the board can continue to do other
// things. Also, because some of the features of the Wifi class seem to require background processing.
void ConnectWifi(char* ssid, char* password)  
{  
    // After a timeout, number of milliseconds to wait until trying again
    static int millisecondsToRetry = DelayPerLoop;
    
    byte attempts = 0;   // Counter for the number of attempts to connect to wireless AP

    millisecondsToRetry -= DelayPerLoop;

    if (millisecondsToRetry <= 0)
    {
      Serial.print(F("Connecting to ")); Serial.println(ssid);
  
      WiFi.begin(ssid, password); // Connect to WiFi network

      while (WiFi.status() != WL_CONNECTED) // Test to see if we're connected
      {
          Serial.print('.');
          attempts++;
    
          if(attempts > 20) // Give up after 20 tries 
          {
            Serial.println (F("\nWifi connection failed. Will try again in 30 seconds."));
            millisecondsToRetry = 30000;
            break; 
          }
          else 
          {
            delay(500);      // Check again after 500ms
          }
      }
  
      if (WiFi.status() == WL_CONNECTED)  // We're connected
      {
         Serial.println(F("\nWiFi connected ...\n"));
      }
      else  // Unable to connect
      {
         WiFi.disconnect();
      }
   }
}
// -------------------------------------------------------
// This function is called during each repetition of loop()
void SerialEvent() 
{
  while (Serial.available()) 
  {
    // Get the new character
    char inChar = (char)Serial.read(); 

    // And add it to the command processor
    TheSerialInterface.Add(inChar);  
  }
}
