/*
Copyright 2019 Scott Henwood All Rights Reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products 
   derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY CANARIE Inc. "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <SensorConfig.h>
#include <EEPROM.h>
#include <Arduino.h>

// ----------------------------------------------------------------------
// Return the one's complement checksum of the configuration structure. This
// checksum is stored in EEPROM along with the configuration itself.
unsigned char SensorConfigClass::CalculateChecksum (void)
{
	unsigned char* configurationBytes = (unsigned char*)&TheConfiguration;
	
	unsigned char returnValue = 0;
	
	for (int i = 0; i < sizeof(TheConfiguration); i++)
	{
		returnValue += *configurationBytes++;
	}
	
	return (0xff - returnValue);
}


// ----------------------------------------------------------------------
// Constructor - allocate EEPROM space
SensorConfigClass::SensorConfigClass (void)
{
	
	// Clear the configuration structure
	memset (&TheConfiguration, 0, sizeof (TheConfiguration));
	
    // Add 1 for the checksum
    EEPROM.begin (sizeof(TheConfiguration)+1);
}
		

// ----------------------------------------------------------------------
// Write configuration information to EEPROM, adding a checksum
void SensorConfigClass::Write (void)
{
    unsigned writeAddr = 0;
	
    unsigned char checksum = CalculateChecksum ();
	
    // Write the data
    EEPROM.put (writeAddr, TheConfiguration);
	
    // Write the checksum
    writeAddr += sizeof (TheConfiguration);
    EEPROM.put (writeAddr, checksum);
	
    EEPROM.commit();
}
		

// ----------------------------------------------------------------------
// Read configuration information from EEPROM and validate the checksum
// Returns true if configuration is valid and false otherwise
bool SensorConfigClass::Read(void)
{
    bool returnValue = true;
    unsigned readAddr = 0;

    // Zero out configuration structure. Helps to null-terminate strings
    memset (&TheConfiguration, 0, sizeof (TheConfiguration));
	
    // Read the data
    EEPROM.get (readAddr, TheConfiguration);
	
    // Calculate the checksum of this data
    unsigned char checksum  = CalculateChecksum ();
	
    // Read the stored checksum
    readAddr += sizeof (config_t);
    unsigned char storedChecksum;
    EEPROM.get (readAddr, storedChecksum);

    if (checksum != storedChecksum)
        returnValue = false;
	
    return (returnValue);
}

// ----------------------------------------------------------------------
// Load the configuration from EEPROM. This must be called after the object is
// created but before any of the other methods can be used. Returns 0 on success and -1
// if something goes wrong.
bool SensorConfigClass::Load (void)
{
    // Read our configuration from EEPROM
    bool returnValue = Read();
    
    // If there was a checksum failure, restore defaults
    if (returnValue == false)
    {
    	Initialize (DEFAULT_UUID,
    		        DEFAULT_HAS_WATER_SENSOR,
    		        DEFAULT_HAS_TEMP_SENSOR,
    		        DEFAULT_HAS_BUZZER,
    		        DEFAULT_WIFI_SSID,
    		        DEFAULT_WIFI_PASSWORD,
    		        DEFAULT_IFTTT_KEY,
    		        DEFAULT_WATER_DETECT_THRESHOLD,
    		        DEFAULT_TEMPERATURE_LOW_THRESHOLD,
    		        DEFAULT_TEMPERATURE_HIGH_THRESHOLD);
    		        
    }
		
    return (returnValue);
}


// ------------------------------------------------------------------------------
// Clears EEPROM and writes the values provided. Intended to be used by
// the initialization sketch to configure boards. 
void SensorConfigClass::Initialize (char* theUUID,
        	            			bool  hasWaterSensor,
        	            			bool  hasTempSensor,
        	            			bool  hasBuzzer,
        	            			char* theWifiSSID, 
        	            			char* theWifiPassword, 
        	            			char* theIFTTTKey,
        	            			int   waterDetectThreshold,
        	            			int   temperatureLowThreshold,
        	            			int   temperatureHighThreshold)
{
    strncpy (TheConfiguration.UUID, theUUID, UUID_LEN);
    TheConfiguration.HasWaterSensor = hasWaterSensor;
    TheConfiguration.HasTempSensor  = hasTempSensor;
    TheConfiguration.HasBuzzer      = hasBuzzer;
	
    strncpy (TheConfiguration.WifiSSID, theWifiSSID, WIFI_SSID_LEN);
    strncpy (TheConfiguration.WifiPassword, theWifiPassword, WIFI_PASSWORD_LEN);
    strncpy (TheConfiguration.IFTTTKey, theIFTTTKey, IFTTT_KEY_LEN);
    
    TheConfiguration.WaterDetectThreshold = waterDetectThreshold;
    TheConfiguration.TemperatureLowThreshold = temperatureLowThreshold;
    TheConfiguration.TemperatureHighThreshold = temperatureHighThreshold;
    
    // Zero out alignment fields we're not currently using
    TheConfiguration.Spare1 = 0;
    TheConfiguration.Spare2 = 0;
    
    // Store the version of our configuration layout
    TheConfiguration.Version = CONFIGURATION_VERSION;

    // And save
    Write();
}

// ------------------------------------------------------------------------------
// Configure the sensor information - name, and what hardware is connected.
void SensorConfigClass::SetSensor (char*  theUUID,
        	             		   bool   hasWaterSensor,
        	             		   bool   hasTempSensor,
        	             		   bool   hasBuzzer)
{
    strncpy (TheConfiguration.UUID, theUUID, UUID_LEN);
    TheConfiguration.HasWaterSensor = hasWaterSensor;
    TheConfiguration.HasTempSensor  = hasTempSensor;
    TheConfiguration.HasBuzzer      = hasBuzzer;

    // And save
    Write();
}

// ------------------------------------------------------------------------------
// Change the water detect threshold. Assumes the value has
// already been clamped to the valid range.
void SensorConfigClass::SetWaterDetectThreshold(int newThreshold)
{
	if ((newThreshold >= MIN_WATER_DETECT_THRESHOLD) 
		&& (newThreshold <= MAX_WATER_DETECT_THRESHOLD) 
		&& (newThreshold != TheConfiguration.WaterDetectThreshold))
	{
		TheConfiguration.WaterDetectThreshold = newThreshold;
		Write();
	}
		
}
 
// ------------------------------------------------------------------------------
// Sets WifiSSID to the supplied value
void SensorConfigClass::SetWifiSSID (char* newID)
{
    strncpy (TheConfiguration.WifiSSID, newID, WIFI_SSID_LEN);
    Write();
}
		
// ------------------------------------------------------------------------------
// Sets Wifi password to the supplied value
void SensorConfigClass::SetWifiPassword (char* newPassword)
{
    strncpy (TheConfiguration.WifiPassword, newPassword, WIFI_PASSWORD_LEN);
    Write();
}
		
// ------------------------------------------------------------------------------
// Sets Wifi password to the supplied value
void SensorConfigClass::SetIFTTTKey (char* newKey)
{
    strncpy (TheConfiguration.IFTTTKey, newKey, IFTTT_KEY_LEN);
    Write();
}
		