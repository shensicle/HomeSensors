#ifndef _1PC_CONFIG_H
#define _1PC_CONFIG_H

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
#include "OnePCAppDefs.h"

class OnePCConfigClass
{
    private:
        // The buffer that stores our configuration, as layed out in EEPROM
        camera_config_t TheConfiguration;
        		
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
  	    OnePCConfigClass (void);
				
  	    // Return a copy of our configuration structure
  	    void GetConfiguration (camera_config_t* theConfiguration);
  	    
  	    // Save a new configuration. Return false if something goes wrong with
  	    // the hardware.
  	    bool SetConfiguration (camera_config_t* theConfiguration);
  	       
		
  	    // Load the configuration from EEPROM. This must be called after the object is
  	    // created but before any of the other methods can be used. Returns true on success and false
  	    // if something goes wrong.
  	    bool Load(void);
		
        // Clears EEPROM and writes the values provided. Used on new boards to
        // initialize to default values. Returns false in case of hardware failure.
        bool Initialize (void);
        
};



#endif