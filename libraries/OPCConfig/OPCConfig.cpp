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
unsigned char OPCConfigClass::CalculateChecksum (void)
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
OPCConfigClass::OPCConfigClass (void)
{
	
	// Clear the configuration structure
	memset (&TheConfiguration, 0, sizeof (TheConfiguration));
	
    // Add 1 for the checksum
    EEPROM.begin (sizeof(TheConfiguration)+1);
}
		

// ----------------------------------------------------------------------
// Write configuration information to EEPROM, adding a checksum
void OPCConfigClass::Write (void)
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
bool OPCConfigClass::Read(void)
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
    readAddr += sizeof (camera_config_t);
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
bool OPCConfigClass::Load (void)
{
    // Read our configuration from EEPROM
    bool returnValue = Read();
    
    // If there was a checksum failure, restore defaults
    if (returnValue == false)
    {
    	Initialize ();	        
    }
		
    return (returnValue);
}


// ------------------------------------------------------------------------------
// Clears EEPROM and writes the values provided. Intended to be used by
// the initialization sketch to configure boards. 
bool OPCConfigClass::Initialize (void)
{

	TheConfiguration.version      = CONFIGURATION_VERSION;
	TheConfiguration.horizRes     = DEFAULT_HORIZ_RES;
	TheConfiguration.vertRes      = DEFAULT_VERT_RES;
	TheConfiguration.captureMode  = DEFAULT_CAPTURE_MODE;
	TheConfiguration.bitDepth     = DEFAULT_BIT_DEPTH;

	// And save
    Write();
}

// ------------------------------------------------------------------------------
// Set and save camera configuration
void OPCConfigClass::SetConfiguration ( camera_config_t* theConfiguration)
{
    memcpy (&TheConfiguration, theConfiguration, sizeof(TheConfiguration));

    // And save
    Write();
}

// ------------------------------------------------------------------------------
// Return a copy of our configuration structure
void OPCConfigClass::GetConfiguration (camera_config_t* theConfiguration)
{
	   memcpy (theConfiguration, &TheConfiguration, sizeof(TheConfiguration));
}
  	    

