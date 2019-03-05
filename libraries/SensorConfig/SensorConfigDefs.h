#ifndef _SENSOR_CONFIGDEFS_H
#define _SENSOR_CONFIGDEFS_H

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


// This version of the configuration structure. Used in future software versions
// to be able to detect and update configuration information from previous versions.
#define CONFIGURATION_VERSION 1

// Lengths for fixed fields in the configuration structure. All multiples
// of 4 to maintain long-word alignment
#define WIFI_SSID_LEN     24
#define WIFI_PASSWORD_LEN 24
#define IFTTT_KEY_LEN     32

#define UUID_LEN          24


// Structure to save configuration for this sketch in EEPROM
typedef struct
{
      unsigned short  	Version;                  // Version of this configuration block - used for future upgrades
	  char  		   	WifiSSID[WIFI_SSID_LEN];
      char 			   	WifiPassword[WIFI_PASSWORD_LEN];
      char 			   	IFTTTKey[IFTTT_KEY_LEN];  
      char				UUID[UUID_LEN];
      bool				HasWaterSensor;
      bool				HasTempSensor;
      bool 				HasBuzzer;
      bool 				Spare1;
      unsigned short 	WaterDetectThreshold;
      short  			TemperatureLowThreshold;
      short  			TemperatureHighThreshold;
      short   			Spare2;
} config_t;


// Default values for configuration items
#define DEFAULT_WIFI_SSID "Your SSID"
#define DEFAULT_WIFI_PASSWORD "Your WIFI Password"
#define DEFAULT_IFTTT_KEY "Your IFTTT Key"
#define DEFAULT_UUID "Unnamed Sensor"

#define DEFAULT_HAS_WATER_SENSOR false
#define DEFAULT_HAS_TEMP_SENSOR  false
#define DEFAULT_HAS_BUZZER       false

#define MIN_WATER_DETECT_THRESHOLD 0
#define MAX_WATER_DETECT_THRESHOLD 1023

#define DEFAULT_WATER_DETECT_THRESHOLD 200
#define DEFAULT_TEMPERATURE_LOW_THRESHOLD 0
#define DEFAULT_TEMPERATURE_HIGH_THRESHOLD 40


#endif  
