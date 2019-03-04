#ifndef _SENSOR_CONFIG_H
#define _SENSOR_CONFIG_H

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

// The definition of the configuration for the current sketch. 
#include <SensorConfigDefs.h>

class SensorConfigClass
{
    private:
        // The buffer that stores our configuration, as layed out in EEPROM
        config_t TheConfiguration;
        		
        // Return the one's complement checksum of the configuration structure
        unsigned char CalculateChecksum (void);
		        
    protected:
        // Write configuration information to EEPROM, adding a checksum
        void Write (void);
		
        // Read configuration information from EEPROM and validate the checksum
        // Returns true if configuration is valid and false otherwise
        bool Read(void);
			
	
  	public:
 	    
  	    // Constructor - allocate EEPROM space
  	    SensorConfigClass (void);
				
 	    // Return a pointer to our stored WifiSSID
  	    char* GetWifiSSID(void)
  	       { return (TheConfiguration.WifiSSID); }
  	       
  	    // Sets WifiSSID to the supplied value
  	    void SetWifiSSID (char* newID);
		
  	    // Return a pointer to our stored Wifi password
  	    char* GetWifiPassword(void)
  	       { return (TheConfiguration.WifiPassword); }
		
  	    // Sets Wifi password to the supplied value
  	    void SetWifiPassword (char* newPassword);
		
  	    // Return a pointer to our stored IFTTT key
  	    char* GetIFTTTKey(void)
  	       { return (TheConfiguration.IFTTTKey); }
		
  	    // Sets IFTTT API key to the supplied value
  	    void SetIFTTTKey (char* newKey);
		
  	    // Return a pointer to our own Board ID
  	    char* GetUUID(void)
  	       { return (TheConfiguration.UUID); }
  	       
  	    // Return true if we are equiped with a water sensor
  	    bool HasWaterSensor (void)
  	    { return (TheConfiguration.HasWaterSensor); }
  	    
  	    // Return true if we are equiped with a temperature sensor
  	    bool HasTemperatureSensor (void)
  	    { return (TheConfiguration.HasTempSensor); }
  	    
  	    // Return true if we are equiped with a buzzer
  	    bool HasBuzzer (void)
  	    { return (TheConfiguration.HasBuzzer); }
  	    
  	    // Return the water detect threshold
  	    int GetWaterDetectThreshold (void)
  	    { return (TheConfiguration.WaterDetectThreshold); }
  	    
  	    // Change the water detect threshold. Assumes the value has
  	    // already been clamped to the valid range.
  	    void SetWaterDetectThreshold(int newThreshold);
  	    
  	    // Return temperature sensor thresholds
  	    int GetTemperatureLowThreshold (void)
  	    { return (TheConfiguration.TemperatureLowThreshold); }
  	    
   	    int GetTemperatureHighThreshold (void)
  	    { return (TheConfiguration.TemperatureHighThreshold); }
  	    
  	    // Toggle VerboseModeOn and return a value indicating whether or not it is on
  	    bool ToggleVerboseMode(void);
		
  	    // Load the configuration from EEPROM. This must be called after the object is
  	    // created but before any of the other methods can be used. Returns true on success and false
  	    // if something goes wrong.
  	    bool Load(void);
		
        // Clears EEPROM and writes the values provided. Intended to be used by
        // the initialization sketch to configure boards. 
        void Initialize (char* theUUID,
        	             bool  hasWaterSensor,
        	             bool  hasTempSensor,
        	             bool  hasBuzzer,
        	             char* theWifiSSID = DEFAULT_WIFI_SSID, 
        	             char* theWifiPassword = DEFAULT_WIFI_PASSWORD, 
        	             char* theIFTTTKey = DEFAULT_IFTTT_KEY,
        	             int   waterDetectThreshold = DEFAULT_WATER_DETECT_THRESHOLD,
        	             int   temperatureLowThreshold = DEFAULT_TEMPERATURE_LOW_THRESHOLD,
        	             int   temperatureHighThreshold = DEFAULT_TEMPERATURE_HIGH_THRESHOLD);
        
        
        // Configure the sensor information - name, and what hardware is connected.
        void SetSensor (char*  theUUID,
        	             bool  hasWaterSensor,
        	             bool  hasTempSensor,
        	             bool  hasBuzzer);
};



#endif