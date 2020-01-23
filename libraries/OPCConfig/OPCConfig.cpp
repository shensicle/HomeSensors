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

#include "OPCConfig.h"
#include <EEPROM.h>
#include <Arduino.h>

// ----------------------------------------------------------------------
// Return the one's complement checksum of the configuration structure. This
// checksum is stored in EEPROM along with the configuration itself.
unsigned char OPCConfigClass::CalculateChecksum (char* theConfiguration)
{
	char* configurationBytes = theConfiguration;
	
	unsigned char returnValue = 0;
	
	for (int i = 0; i < ConfigLength; i++)
	{
		returnValue += *configurationBytes++;
	}
	
	return (0xff - returnValue);
}


// ----------------------------------------------------------------------
// Constructor - allocate EEPROM space
OPCConfigClass::OPCConfigClass (unsigned short configLength)
{
	ConfigLength = configLength;

    // Add 1 for the checksum
//    EEPROM.begin (ConfigLength+1);

}
		

// ----------------------------------------------------------------------
// Write configuration information to EEPROM, adding a checksum
void OPCConfigClass::Write (char* theConfiguration)
{
    unsigned writeAddr = 0;
	
    unsigned char checksum = CalculateChecksum (theConfiguration);
	
    // Write the data
    EEPROM.put (writeAddr, theConfiguration);
	
    // Write the checksum
    writeAddr += ConfigLength;
    EEPROM.put (writeAddr, checksum);
	
//    EEPROM.commit();
}
		

// ----------------------------------------------------------------------
// Read configuration information from EEPROM and validate the checksum
// Returns true if configuration is valid and false otherwise
bool OPCConfigClass::Read(char* theConfiguration)
{
    bool returnValue = true;
    unsigned readAddr = 0;

    // Zero out configuration structure. Helps to null-terminate strings
    memset (theConfiguration, 0, ConfigLength);
	
    // Read the data
    EEPROM.get (readAddr, theConfiguration);
	
    // Calculate the checksum of this data
    unsigned char checksum  = CalculateChecksum (theConfiguration);
	
    // Read the stored checksum
    readAddr += ConfigLength;
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
bool OPCConfigClass::LoadConfiguration (char* theConfiguration, char* defaultConfiguration)
{
    // Read our configuration from EEPROM
    bool returnValue = Read(theConfiguration);
    
    // If there was a checksum failure, restore defaults
    if (returnValue == false)
    {
    	memcpy (theConfiguration, defaultConfiguration, ConfigLength);
    	Write (theConfiguration);  
    }
		
    return (returnValue);
}

