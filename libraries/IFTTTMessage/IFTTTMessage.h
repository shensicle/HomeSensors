#ifndef _IFTTT_MESSAGE_H
#define _IFTTT_MESSAGE_H

#include <Arduino.h>

#include <WiFiServerSecure.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiType.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFiAP.h>
#include <WiFiClient.h>
#include <BearSSLHelpers.h>
#include <WiFiServer.h>
#include <ESP8266WiFiScan.h>
#include <WiFiServerSecureBearSSL.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiSTA.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiServerSecureAxTLS.h>


class IFTTTMessageClass
{
  private:
    char* APIKey;
    WiFiClient TheClient;
    
    // String used for static parts of message sent to IFTTT
    String PostString;    
    
    // String used to hold the first label of the JSON packet, typically a
    // unique identifier for the sensor host
    String SensorID;
    
    // Connect to the ifttt service. Returns true if connection was successful
    virtual bool Connect (void);



  public:
   // Constructor - doens't do much because we have to wait until configuration
   // is loaded from the personality file before initializing this object
   IFTTTMessageClass (void);

   // Initialize - pass in API key for IFTTT and a tag to use in the JSON packet,
   // which is typically a unique identifier for this host. This can't be done in
   // constructor as we have to wait for personality to be read from EEPROM. Call
   // this method once before calling Send()
   void Initialize (const char* theAPIKey, const char* sensorID);

    // Send a message. Return value indicates whether or not message was successfully sent
    virtual bool Send (String theMessage);
    
};

#endif
