#include "SD.h"
#include "Stepper.h"

#include "OPCApp.h"

// -----------------------------------------------------------------------------	
void OPCap::OPCap (void)
{
    // Set pins for motors
    
    // Set pins for sensor platform limit switches
    
    // Set pins for SD card
    // if (! SD.begin(...) {};
    
    LoadNextFileNumber (void);
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
// and current camera configuration is not modified.                                                     
bool OPCApp::SetCameraConfig (camera_config_t* theConfig)
{
}
	
// -----------------------------------------------------------------------------	
// Get the current camera configuration
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
	
	// Open the output file
	
	// Write the file header
	
	// Start the capture thread
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
        CaptureFile.close();  // SHOULD BE DONE IN TASK ??
	    
	    CaptureUnderway = false;
	}
}

// -----------------------------------------------------------------------------	
// Figure out the name (number) of the next file we can write to
void OPCApp::LoadNextFileNumber (void)
{
	// Attempt to open file LastNumber.txt. If this file can't be opened, start
	// numbering images at 1. Otherwise, use the number stored in this file
	
	// Try to open file
	NameFile = SD.open(NAME_FILE);
	
	// If successful, read the number on the first line
	NextFileNumber = 0;
	if (NameFile)
	{
	    if NameFile.available())
	    {
	        NextFileNumber = NameFile.parseInt();
	    }
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
    NameFile = SD.open (NAME_FILE, O_WRITE);
    
    if (NameFile)
    {
        NameFile.print (NextFileNumber);
        NameFile.close();
    }
    else
    {
        // Flag error for user interfaces
    }
}


// -----------------------------------------------------------------------------	
// Write the header at the start of an image file
void OPCApp::WriteImageFileHeader(void)
{
}

// -----------------------------------------------------------------------------		
// Move the sensor to the first pixel position of the image
void OPCApp::MoveSensorHome (void)
{
}

// -----------------------------------------------------------------------------	
//void OPCApp::CaptureTask (void)
//{
	// For each row in the image
	
		// For each column in the current row
		
		// Move sensor
		
		// Read sensor
		
		// Save sensor value
		
		// Check to make sure an abort hasn't been requested
//}
	
