
#include <Personality.h>

// Passwords and other sensitive data that I don't want in git
#include "Passwords.h"

// The class that manages saving/loading personality information to/from EERPOM
PersonalityClass ThePers;


// ---------------------------------------------------------------------------------
void setup() {
  
  static personality_t pers, saved;
  memset (&pers, 0, sizeof (pers));
  memset (&saved, 0, sizeof (saved));


  // The source strings below are defined in Passwords.h
  strcpy ((char*)&pers.WifiSSID, wifiSSID);
  strcpy ((char*)&pers.WifiPassword, wifiPassword);
  strcpy ((char*)&pers.IFTTTKey, iftttKey);  

  // Set these as appropriate
  strcpy ((char*)&pers.UUID, "Sensor1");  // UUID is used in messages to identify the board that generated the message 
  pers.HasWaterSensor = true;             // board has a water sensor
  pers.HasTempSensor = false;             // board has a temperature sensor
  pers.HasBuzzer = true;                  // board has an alarm buzzer

  // Print out data before we write it to EEPROM
  Serial.println ("About to write the following to EEPROM .../n");
  DumpPersonality (&pers);

  // And save it
  ThePers.Write (&pers);

  // As a check, read it back and validate
  bool okay = ThePers.Read(&saved);
  if (okay == true)
  {
      // Checksum was good; Verify the contents
      if (memcmp (&pers, &saved, sizeof(pers)) != 0)
      {
         Serial.println ("\n*** Personality readback comparison failed\n");
         DumpPersonality (&saved); 
      }
      else

         Serial.println ("\nPersonality successfully saved\n");
  }
  else
    Serial.println ("\n*** Personality checksum mismatch\n");

      
}

// ---------------------------------------------------------------------------------
void loop() 
{
  // Don't need to do anything here
}

// ---------------------------------------------------------------------------------
// Print the passed-in personality data in a human-friendly format
void DumpPersonality (personality_t* persData)
{
  Serial.println ("Writing the following personality to EERPOM:");
  Serial.print ("WIFI SSID: "); Serial.println (persData->WifiSSID);
  Serial.print ("WIFI Password: "); Serial.println (persData->WifiPassword);
  Serial.print ("IFTTT API Key: "); Serial.println (persData->IFTTTKey);
  Serial.print ("Sensor ID: "); Serial.println (persData->UUID);

  Serial.print ("Water sensor: ");
  if (persData->HasWaterSensor)
    Serial.println ("Yes");
  else
    Serial.println ("No");
    
  Serial.print ("Temperature sensor: ");
  if (persData->HasTempSensor)
    Serial.println ("Yes");
  else
    Serial.println ("No");
    
  Serial.print ("Alarm Buzzer: ");
  if (persData->HasBuzzer)
    Serial.println ("Yes");
  else
    Serial.println ("No");

  Serial.println();

}
