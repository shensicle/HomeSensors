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
// Process the S (set) command
void SensorSerialInterface::ProcessSCommand(void)
{
	static char sensorID[UUID_LEN];
	int status = 0;
	bool hasWaterSensor = false;
	bool hasTempSensor = false;
	bool hasBuzzer = false;
	
    // First get the sensor ID - The only criterion is that it
    // can't be blank.
    Parser.GetStringToWhitespace (sensorID, UUID_LEN);
      					
    if (sensorID[0] == 0x00)
    {
      	// 'S' command only - display current configuration
      	Serial.println (F("\nCurrent configuration:"));
      	Serial.print (F("Sensor ID: ")); Serial.println (TheConfiguration->GetUUID());
      	Serial.print (F("Water Sensor: ")); 
      	if (TheConfiguration->HasWaterSensor())
      	    Serial.println ("Yes");
      	else
      	    Serial.println ("No");

       	Serial.print (F("Temperature Sensor: ")); 
      	if (TheConfiguration->HasTemperatureSensor())
      	    Serial.println ("Yes");
      	else
      	    Serial.println ("No");

      	Serial.print (F("Warning Buzzer: ")); 
      	if (TheConfiguration->HasBuzzer())
      	    Serial.println ("Yes\n");
      	else
      	    Serial.println ("No\n");
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
     
      	// If something went wrong ...
      	if (status == -1)
      	    Serial.println (F("Error: Must specify at least one sensor\n"));
      	else
      	{
      	    // Configure this sensor
      	    TheConfiguration->Initialize (sensorID, hasWaterSensor, hasTempSensor, hasBuzzer);
      	}
     }

}

// --------------------------------------------------------------------------- 
// Process the W (water level threshold) command
void SensorSerialInterface::ProcessWCommand(void)
{
    // If there is more data on the command line, assume the threshold has been
    // specified and we are doing a set.
    if (Parser.MoreDataAvailable())
    {
     	int newThreshold = Parser.GetUnsignedLong();

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
     else
     {
     	// Nothing on the command line other than the W command, so we are doing a get
     	Serial.print (F("Current water sensor threshold: "));
     	Serial.print (TheConfiguration->GetWaterDetectThreshold()); Serial.println ("\n");
     }
}

// --------------------------------------------------------------------------- 
// Process the C (set communcations paramenters) command
void SensorSerialInterface::ProcessCCommand(void)
{
    // Storage for local copies of settable items. We only change things
    // it there are no syntax errors in the comand.
    const int bufLen = IFTTT_KEY_LEN;  // IFTTT_KEY_LEN is the longest of the
    static char newString[bufLen];     // configuation items dealt with in this file
                                           
     	    
    char nextCmd = Parser.GetCommandAndParameter(newString, bufLen);
     	    
    // If there is nothing on the command line, just display
    if (nextCmd == 0x00)
    {
        Serial.println (F("\nCommunications Parameters:"));
        Serial.print (F("Wifi SSID: ")); Serial.println (TheConfiguration->GetWifiSSID());
        
        char* temp = TheConfiguration->GetWifiPassword();
        Serial.print (F("Wifi Password: "));
        Serial.print(temp[0]);
        Serial.println(F("***********"));
         
        
        temp = TheConfiguration->GetIFTTTKey();
        Serial.print (F("IFTTT API Key: "));
        Serial.print(temp[0]);
        Serial.println(F("***********"));
        Serial.println("\n");
    }
    
    // If there was no command, this will get skipped - no need for an 'else'
    while (nextCmd != 0x00)
    {
     	 switch (toupper(nextCmd))
     	 {
     	      case 'S':   // Wifi SSID
     	          TheConfiguration->SetWifiSSID (newString);
     	          break;
     	          
     	      case 'P':   // Wifi password
     	          TheConfiguration->SetWifiPassword (newString);
     	          break;
     	          
     	      case 'A':   // ifttt.com API key
     	          TheConfiguration->SetIFTTTKey (newString);
     	          break;
     	          
     	      default:
     	          Serial.print (F("\nError: Invalid command - ")); 
     	          Serial.print (nextCmd); Serial.println (F(" - ignored - Type H for help\n"));
     	          break;
     	 }
     	 
     	 nextCmd = Parser.GetCommandAndParameter(newString, bufLen);   

     }
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
      		    ProcessSCommand();					
      			break;
        
      		// Display a help message
      		case 'H':
      			Serial.println (F("S [<sensor name> <WTB>] - sets the sensor configuration"));
      			Serial.println (F("where W, T and B denote the presence of: "));
      			Serial.println (F("  - a water sensor (W)"));
      			Serial.println (F("  - a temperature sensor (T)"));
      			Serial.println (F("  - a buzzer (B)"));
      			Serial.println (F("  - if there are no parameters, the current sensor information is displayed\n"));
      			Serial.println (F("\nW [<threshold>] - sets the water sensor detection threshold."));
      			Serial.println (F("  - <threshold> must be in range [0..1023] and should be determined experimentally"));
      			Serial.print   (F("  - default value is ")); Serial.println (DEFAULT_WATER_DETECT_THRESHOLD);
      			Serial.println (F("  - If <threshold> is not specified, the current value is displayed\n"));
      			Serial.println (F("\nC [-s <wifi ssid>] [-p <wifi password>] [-a <IFTTT API key>]"));
      			Serial.println (F("  - Allows user to specify communications parameters if the defaults are not acceptable"));
      			Serial.println (F("  - if there are no parameters, current communications settings are displayed.\n"));
      			Serial.println (F("\nH - displays this message\n"));
      			break;
        
      		// Change the water detection threshold for hosts equipped with a water sensor	
     		case 'W':
     		    ProcessWCommand();
      			break;
     			
     		// Set the communications parameters - this is compiled in for convenience,
     		// but if you have to deploy a small number of sensors in a new location,
     		// this might be a faster way to do it.
     	  case 'C':
     	      ProcessCCommand();   	    
     	      break;

    			
     	  default:
     	      Serial.println (F("\nError: Invalid command - Type H for help\n"));
     	      break;

      	}
    
      	CommandComplete = false;
      	InputString = ""; 
      }  
}


