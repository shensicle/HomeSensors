
#include "IFTTTMessage.h"

// -----------------------------------------------------
IFTTTMessageClass::IFTTTMessageClass (void)

{
}

// -----------------------------------------------------
// Initialize - pass in API key for IFTTT and a tag to use in the JSON packet,
// which is typically a unique identifier for this host. This can't be done in
// constructor as we have to wait for personality to be read from EEPROM. Call
// this method once before calling Send()
void IFTTTMessageClass::Initialize (const char* theAPIKey, const char* sensorID)

{
    SensorID = sensorID;
    PostString =   "POST /trigger/sensor_alert/with/key/";
    PostString += theAPIKey;
    PostString += " HTTP/1.1\nHost: maker.ifttt.com\nUser-Agent: ShensicleSensor\nConnection: close\nContent-Type: application/json\nContent-Length: ";
}

// -----------------------------------------------------
// Connect to the ifttt service.
bool IFTTTMessageClass::Connect (void)
{
   // Value which, when set, indicates connection to IFTTT server was successful
   bool returnValue = true;
   
   if(TheClient.connect("maker.ifttt.com",80))  // Test the connection to the server
   {
     Serial.println(F("Connected to ifttt"));
   }
   else
   {
     Serial.println(F("Failed to connect to ifttt"));
     returnValue = false;
   }
   
   return (returnValue);
}

// -----------------------------------------------------
// Send a message. Return value indicates whether or not message was successfully sent
bool IFTTTMessageClass::Send (String theMessage)
{
    
    if (Connect())
    {
    	// Note that ifttt only supports labels value1, value2, value3
  	    String postData;
  	    postData.concat ("{\"value1\":\"");
   	    postData.concat (SensorID);
  	    postData.concat ("\",\"value2\":\"");
  	    postData.concat(theMessage);
  	    postData.concat("\"}");

  	    TheClient.print (PostString);          // Connection details
	    TheClient.println(postData.length());  // length of JSON payload
        TheClient.println();
        TheClient.println(postData);           // JSON payload
        
        Serial.println (F("Message sent to ifttt\n"));
    }
    else
    {
         Serial.println (F("Unable to send to ifttt\n"));
   }
}

