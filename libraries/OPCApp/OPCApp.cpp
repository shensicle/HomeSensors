#include <arduino.h>

#include <SD.h>
#include <Stepper.h>

#include "OPCApp.h"


#define NAME_FILE "name.txt"


// Header for the OPC raw image format. Very similar to PNG header
static char FileHeader[] = { 
	           0x89,    // High bit set to avoid confusion between OPC and text files
               'O', 'P', 'C',
               0x0d, 0x0a,      // Windows CR/LF
               0x1a,            // EOF - causes DOS 'type' command to stop here
               0x0a };          // Unix LF
#define FILE_HEADER_LEN 8
               
// -----------------------------------------------------------------------------	
OPCApp::OPCApp (BH1750FVI* theLightMeter, camera_config_t* defaultConfiguration)
    : ConfigurationManager (sizeof(camera_config_t))
{
    // Set pins for motors
    
    // Set pins for sensor platform limit switches
    
    // Set pins for SD card
    // SHOULD PROBABLY DO ALL OF THE ABOVE IN THE SKETCH AND PASS IN OBJECTS
    
    // if (! SD.begin(...) {};
 
    
    TheLightMeter = theLightMeter;
    
    if (ConfigurationManager.LoadConfiguration ((char*)&TheConfiguration, (char*)defaultConfiguration) != true)
    {
    	// Flag an error
    }
    
    // Should deal with low and high sensor resolution here !!!
    
    // Commented out for testing
    // LoadNextFileNumber ();
    
    MoveSensorToHome();
}

// -----------------------------------------------------------------------------	
// Set the camera resolution. Return value indicates success when set.
bool OPCApp::SetCameraResLow(void)
{
}

// -----------------------------------------------------------------------------	
bool OPCApp::SetCameraResHi(void)
{
}
	
// -----------------------------------------------------------------------------	
// Set the camera configuration. If parameters are invalid, a false is returned
// and current camera configuration is not modified. Used by user interfaces.                                                    
bool OPCApp::SetCameraConfig (camera_config_t* theConfig)
{
}
	
// -----------------------------------------------------------------------------	
// Get the current camera configuration. Used by user interfaces.
void OPCApp::GetCameraConfig (camera_config_t* theConfig)
{
}
	
	
// -----------------------------------------------------------------------------	
// Kick off the picture-taking process. Once the picture starts, a task running
// on the other core will manage the capture. If something goes wrong, false
// is returned.
bool OPCApp::StartNewCapture (void)
{
	
	// If there is already a capture in progress, abort it
	AbortCapture();

	// Ensure sensor is in the right place
	MoveSensorHome();
	
	NextRow = NextColumn = 0;
	
	// Open the output file
	ImageFile = SD.open (GetImageFileName(), FILE_WRITE);
	
	if (ImageFile)
		{ Serial.print ("Image file "); Serial.print (GetImageFileName()); Serial.println (" opened okay"); }
	else
		{ Serial.print ("Image file "); Serial.print (GetImageFileName()); Serial.println (" open failed"); }
		
	
	// Write the file header
	bool returnValue = WriteImageFileHeader();

	
	if(returnValue)
	{
	    // Start the capture thread
	    CaptureUnderway = true;
	}
	return (returnValue);
}
	
// -----------------------------------------------------------------------------	
// Abort a capture in progress
void OPCApp::AbortCapture (void)
{
	// Only do something if there is currently a capture underway
	if (CaptureUnderway)
	{
	    // Signal the capture thread to stop and wait until it does
	
	    // Delete the output file from the aborted capture. Don't change the output
	    // file name/number here. We will re-use it.
        ImageFile.close();  // SHOULD BE DONE IN TASK ??
	    
        MoveSensorHome();
        
	    CaptureUnderway = false;
	}
}

// -----------------------------------------------------------------------------	
// Figure out the name (number) of the next file we can write to
void OPCApp::LoadNextFileNumber (void)
{
	// Attempt to open file LastNumber.txt. If this file can't be opened, start
	// numbering images at 1. Otherwise, use the number stored in this file
	
	Serial.println ("Loading file number");
	// Try to open file
	NameFile = SD.open(NAME_FILE);
	
	// If successful, read the number on the first line
	NextFileNumber = 0;
	if (NameFile)
	{
		Serial.println ("Name file exists");
	    if (NameFile.available() >= sizeof(NextFileNumber))
	    {
	    	Serial.println ("Data in name file");
			NameFile.read ((char*)&NextFileNumber, sizeof(NextFileNumber));
			Serial.print ("Next file name is ");Serial.println (NextFileNumber);
	    }
	    else
	    	Serial.println ("No data in name file");
	}
    
	NameFile.close();
	
	// If something went wrong ...
	if (NextFileNumber == 0)
	{
        // Flag an error for user interfaces to fetch
        
        NextFileNumber = 1;
        SaveNextFileNumber();
    }

	
}

// -----------------------------------------------------------------------------		
// Save the number of the next file we can write so that we can use it on
// power-up. This number is turned into a string and used to name the file.
void OPCApp::SaveNextFileNumber (void)
{
	SD.remove(NAME_FILE);   // because seek, truncate and open for truncate aren't implemented in 8266
    NameFile = SD.open (NAME_FILE, FILE_WRITE);
    
    Serial.print ("Saving name file ");Serial.println (NAME_FILE);
    if (NameFile)
    {
    	NameFile.write ((char*)&NextFileNumber, sizeof(NextFileNumber));
        NameFile.close();
    }
    else
    {
        // Flag error for user interfaces
        Serial.println ("Name file save failed");
    }
}


// -----------------------------------------------------------------------------	
// Write the header at the start of an image file
bool OPCApp::WriteImageFileHeader(void)
{
    bool returnValue = false;
    
    if (ImageFile.write((unsigned char*)FileHeader,FILE_HEADER_LEN) == FILE_HEADER_LEN)
    {
    	Serial.println ("Now writing image configuration");
    	
    	int bytesWritten = ImageFile.write((unsigned char*)&TheConfiguration, sizeof(TheConfiguration));
    	Serial.print ("Wrote "); Serial.print (bytesWritten); Serial.println (" bytes of config data.");
    	Serial.print ("Should have written ");Serial.println (sizeof(TheConfiguration));
        if (bytesWritten == sizeof(TheConfiguration))
 //       if (ImageFile.write((unsigned char*)&TheConfiguration, sizeof(TheConfiguration)) == sizeof(TheConfiguration))
            returnValue = true;
    }
    return (returnValue);
}

// -----------------------------------------------------------------------------		
// Move the sensor to the first pixel position of the image
void OPCApp::MoveSensorHome (void)
{
    // If we're already home, don't need to do anything
    
    // Otherwise, if we know where we are, we can move the sensor quickly in
    // large steps
    
    // Otherwise, take a cautious approach and move the sensor slowly until the 
    // home pins are activated. When we're done, flag that we know where we are.
    // This is only likely to happen on power-off while the sensor is not home.
    
}

// -----------------------------------------------------------------------------		
// Return a pointer to the name of the next image file to save
char* OPCApp::GetImageFileName (void)
{
    static char fileName[15]; // A count of up to 4,000,000,000 plus ".OPC" plus null terminator
    
    sprintf (fileName, "IMG_%d\.OPC", NextFileNumber);
    return (fileName);
}

// -----------------------------------------------------------------------------	
void OPCApp::CaptureTask (void)
{
	
	// Don't do anything if we're not capturing an image
	if (CaptureUnderway == false)
		return;
	
	// Sensor was moved home before the capture started, so just take a reading
	// at current location and then move to the next pixel.
	
	// Read sensor
	unsigned short lux = TheLightMeter->GetLightIntensity();
//	Serial.print ("Read "); Serial.println(lux);
		
	// Save sensor value
	ImageFile.write((char*)&lux, sizeof(lux));

	// If we are currently on an even-numbered row
	if ((NextRow % 2) == 0)
	{
	    // Move in the positive direction
	    NextColumn++;
	    
	    // If we're at the end of the row
	    if (NextColumn == TheConfiguration.imageWidth)
	    {
	        // Move back one pixel so will start the odd row at the right
	        NextColumn --;
	        
	        // Move to the next row
	        NextRow ++;
	    }
	}
	else      // we are currently on an odd-numbered row
	{
	    if (NextColumn == 0)
	    {
	        // We're at the right column - advance to the next row
	        NextRow++;
	    }
	    else
	    {
	        NextColumn --;
	    }
	}
	
    // In either case, if we have captured all of the rows ...	
	if (NextRow >= TheConfiguration.imageHeight)
	{
		Serial.println ("We're done");
		// We're done
		AbortCapture();
		
		NextFileNumber++;
		SaveNextFileNumber();
		return;
	}
}
	
// -----------------------------------------------------------------------------	
void OPCApp::DumpConfig (void)
{
		Serial.println ("Configuration loaded");
        Serial.print ("version: "); Serial.println (TheConfiguration.version);
		Serial.print ("imageWidth: "); Serial.println (TheConfiguration.imageWidth);
		Serial.print ("imageHeight: "); Serial.println (TheConfiguration.imageHeight);
		Serial.print ("bitDepth: "); Serial.println (TheConfiguration.bitDepth);
		Serial.print ("colourType: "); Serial.println (TheConfiguration.colourType);
		Serial.print ("filterMethod: "); Serial.println (TheConfiguration.filterMethod);
		Serial.print ("scanType: "); Serial.println (TheConfiguration.scanType);
		Serial.println();
}
