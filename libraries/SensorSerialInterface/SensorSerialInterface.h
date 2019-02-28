#ifndef _SENSOR_SERIALINTERFACE_H
#define _SENSOR_SERIALINTERFACE_H

/*
Copyright � 2019 Scott Henwood/shensicle photographic. All Rights Reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products 
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Scott Henwood/shensicle photographic "AS IS" AND 
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

#include <String.h>
#include "CmdParser.h"
#include "SensorConfig.h"
#include "WaterSensor.h"

class SensorSerialInterface
{
protected:
	// String to store received characters
	String InputString;
	
	// Parser object
	CmdParser Parser;
	
	// Sensor configuration
	SensorConfigClass* TheConfiguration;

	// A flag which, when set, indicates that we have received a complete
	// command - ie. last character was a line feed
	bool CommandComplete;
	
	// The number of supported profiles
	int NumProfiles;
	
	// Methods to process individual terminal commands, to keep Update() reasonable
	void ProcessCCommand(void);
	void ProcessSCommand(void);
	void ProcessWCommand(void);
	
public:
	// Constructor
	SensorSerialInterface (SensorConfigClass* theConfiguration);
	
	// Add a character to the command currently being built up
	void Add (char inChar);
	
	// If we have a complete command, parse and act on it
	void Update (void);
};

#endif
