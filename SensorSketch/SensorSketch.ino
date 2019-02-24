#include <ESP8266WiFi.h>

#include <IFTTTMessage.h>

#include <WaterSensor.h>
#include "SensorConfigDefs.h"
#include "SensorConfig.h"
#include "SensorSerialInterface.h"
#include "SimpleLED.h"


// Constants we use in the loop() method to serivce hardware at different intervals
unsigned int DelayPerLoop         = 50;  // milliseconds - has to be fast enough for terminal interface
const unsigned int WaterSensorUpdateInterval = 10000;  // check water sensor every 10 seconds
const unsigned int TempSensorUpdateCount  = 10000;  // check temperature sensor every 10 seconds

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

// Buzzer stuff
//const int TheBuzzerPin = D4;          // Digital IO pin for buzzer

// Object that manages the terminal interface
SensorSerialInterface TheSerialInterface(&TheConfiguration);


// --------------------------------------------------------------------------------------------------
void setup() {

  // Start serial communication for debug information
  Serial.begin(115200); 

  Serial.println ("\nshensicle sensor platform - type 'H' for help\n");
  
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
  }

  else
  {
      ConfigurationIsValid = false;
      
      // Speed up LED flashing to alert the user
      TheLED.SetUpdateInterval(100);
      
      Serial.println ("*** Configuration checksum failure ***");
      Serial.println ("Use the 'S' command to set configuration\n");
  }
}

// --------------------------------------------------------------------------------------------------
void loop()
{  
  
  // Check the serial interface for a complete command and, if there is one, execute it
  TheSerialInterface.Update();

  // If our configuration is not valid, we an't do much except wait for one on the terminal interface
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

       // if (TheConfiguration.HasTemperatureSensor())
          //TheTemperatureSensor.Service();
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
bool ConnectWifi(char* ssid, char* password)  // Tries to connect to the wireless access point with the credentials provided
{
    bool timeOut = 0; // Change to 1 if connection times out
    byte attempts = 0;   // Counter for the number of attempts to connect to wireless AP
  
    Serial.print("Connecting to ");
    Serial.println(ssid);
  
    WiFi.begin(ssid, password); // Connect to WiFi network

    while (WiFi.status() != WL_CONNECTED && (timeOut == 0)) // Test to see if we're connected
    {
        Serial.print('.');
        attempts++;
    
        if(attempts > 60) 
            break; // Give up after ~30 seconds
        else 
            delay(500);      // Check again after 500ms
    }
  
    if (WiFi.status() == WL_CONNECTED)  // We're connected
    {
        Serial.println("\r\nWiFi connected");
    }
    else  // Unable to connect
    {
        WiFi.disconnect();
        Serial.println("\r\nConnection Timed Out!\r\n");
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


