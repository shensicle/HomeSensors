/*
Copyright 2019 Scott Henwood/shensicle photographic. All Rights Reserved.

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


#include <Arduino.h>

#include "WaterSensor.h"


// -----------------------------------------------------------------------------
WaterSensorClass::WaterSensorClass ( const unsigned char sensorPin,
    	             				 unsigned long serviceInterval, 
    	             				 unsigned long updateInterval,
    	             				 IFTTTMessageClass*  theIFTTTSender,
    	             				 SensorConfigClass*  theConfiguration) 
	: PeriodicService (serviceInterval, updateInterval)
{
  SensorPin = sensorPin;
  TheConfiguration = theConfiguration;

  IFTTTSender = theIFTTTSender;
}

// -----------------------------------------------------------------------------
void WaterSensorClass::Update (void)
{
	static bool waterDetectedPreviously = false;
    static bool waterDetected = false;
    
    static char msgString[30];   // keep it off the stack
      

	// Read the analog pin - returns a value between 0 and 1023
	int analogValue = analogRead(SensorPin);
	
	Serial.print ("Water Sensor Reading: "); Serial.println (analogValue);
	
	// Analog pin returns higher value when sensor is wet
	if (analogValue > TheConfiguration->GetWaterDetectThreshold())
	    waterDetected = true;
	else
	    waterDetected = false;


    // Now run through the FSM. We don't need to do anything if the state hasn't changed since last time
    if (waterDetected != waterDetectedPreviously)
    {
    	if (waterDetected == false)
    	    sprintf (msgString, "No Water detected (%d)", analogValue);
    	else
     	    sprintf (msgString, "Water detected (%d)", analogValue);
   	
    	IFTTTSender->Send (msgString);
    	waterDetectedPreviously = waterDetected;
    }
}
