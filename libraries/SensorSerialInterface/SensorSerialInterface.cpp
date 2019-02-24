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
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIGOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <SensorSerialInterface.h>


// Size of buffer for incoming serial characters
#define BUF_SIZE 80

	
// --------------------------------------------------------------------------- 
// Constructor
SensorSerialInterface::SensorSerialInterface (SensorConfigClass* theConfiguration)
	: Parser(&InputString) 
{ 
	InputString = ""; 
	CommandComplete = false; 
	
	TheConfiguration = theConfiguration;
	
	// Reserve space for incoming commands
    InputString.reserve(BUF_SIZE);

}
	
// --------------------------------------------------------------------------- 
// Add a character to the command currently being built up
void SensorSerialInterface::Add (char inChar)
{
	InputString += inChar;
	
	if (inChar == '\n')
		CommandComplete = true;
}
	
// --------------------------------------------------------------------------- 
// If we have a complete command, parse and act on it
void SensorSerialInterface::Update (void)
{
	// temporary storage for configuration items
	static char sensorID [UUID_LEN];
	bool hasWaterSensor = false;
	bool hasTempSensor = false;
	bool hasBuzzer = false;
	
	int status = 0;
	
	if (CommandComplete == true)
	{
		Parser.Reset();
		
		char command = Parser.GetChar();
		command = toupper(command);
		    
		switch (command)
		{
			// The Set command - format is 'S' <sensor name> [WTB] where W, T and B
      		// denote the presence of a water sensor, a temperature sensor and a
      		// buzzer respectively
      		case 'S':
                        
      			// First get the sensor ID - The only criterion is that it
      			// can't be blank.
      			Parser.GetStringToWhitespace (sensorID, UUID_LEN);
      					
      			if (sensorID == 0x00)
      			{
      				Serial.println ("Error: Incorrect command syntax - Type H for help\n");
      				status = -1;
      			}
      			else
      			{
      				// Now start looking for individual characters that describe the
      				// hardware this sensor has.
      				char returnValue = Parser.GetChar();
      						
      				status = -1;
      				while (returnValue != 0x00)
      				{
      					switch (toupper(returnValue))
      					{
      						case 'W':
      							status = 0;
      							hasWaterSensor = true;
      							break;
      							
      						case 'T':
      							status = 0;
      							hasTempSensor = true;
      							break;
      							     							
      						case 'B':
      							// Note we don't set status to 0 here because
      							// need at least one sensor
      							hasBuzzer = true;
      							break;
      					}
      							
      					returnValue = Parser.GetChar();
      				}
      			}		
      			// If something went wrong ...
      			if (status == -1)
      				Serial.println ("Error: Must specify at least one sensor\n");
      			else
      			{
      				// Configure this sensor
      				TheConfiguration->Initialize (sensorID, hasWaterSensor, hasTempSensor, hasBuzzer);
      			}
     								
      			break;
        
      		// Display a help message
      		case 'H':
      			Serial.println ("S <sensor name> [WTB] - sets the sensor configuration");
      			Serial.println ("where W, T and B denote the presence of: ");
      			Serial.println ("- a water sensor (W)");
      			Serial.println ("- a temperature sensor (T)");
      			Serial.println ("- a buzzer (B)");
      			Serial.println ("\nH - displays this message\n");
      			break;
        
      		// Change the water detection threshold for hosts equipped with a water sensor	
     		case 'W':
     			{
     				int newThreshold = Parser.GetUnsignedLong();
     				Serial.print ("WDT: ");Serial.println (newThreshold);
     				if ((newThreshold >= MIN_WATER_DETECT_THRESHOLD) 
     					&& (newThreshold <= MAX_WATER_DETECT_THRESHOLD))
     				{
     					Serial.print ("Water detection threshold set to "); Serial.println(newThreshold);	
     					TheConfiguration->SetWaterDetectThreshold(newThreshold);
     				}
     				else
     				{
     					Serial.print ("\nError: Water detection threshold must be between ");
     					Serial.print (MIN_WATER_DETECT_THRESHOLD); Serial.print (" and ");
     					Serial.println (MAX_WATER_DETECT_THRESHOLD);
     				}
     			}
     			break;
    			
       		default:
      			Serial.println ("\nError: Invalid command - Type H for help\n");
      			break;

      	}
    
      	CommandComplete = false;
      	InputString = ""; 
      }  
}


