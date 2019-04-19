#ifndef _OPCPCAPP_H
#define _OPCAPP_H

#include <SD.h>
#include <BH1750FVI.h>

#include "OPCAppDefs.h"
#include "OPCConfig.h"

// One-pixel camera application

class OPCApp
{
public:
	
	// Constructor - set everything up
	OPCApp (BH1750FVI* theLightMeter, camera_config_t* defaultConfiguration);
	
	// Set the camera resolution. Return value indicates success when set.
	bool SetCameraResLow(void);
	bool SetCameraResHi(void);
	
	// Set the camera configuration. If parameters are invalid, a false is returned
	// and current camera configuration is not modified.
	bool SetCameraConfig (camera_config_t* theConfig);
	
	// Get the current camera configuration
	void GetCameraConfig (camera_config_t* theConfig);
	
	
	// Kick off the picture-taking process. Once the picture starts, a task running
	// on the other core will manage the capture. If something goes wrong, false
	// is returned.
	bool StartNewCapture (void);
	
	// Abort a capture in progress
	void AbortCapture (void);
	
	// Returns a flag which, when set, indicates that there is currently a capture
	// in progress
	bool CaptureInProgress (void)
	{ return (CaptureUnderway); }		
	
	// For testing
	void DumpConfig (void);
	
protected:
    
    // Pointer to the light meter object
    BH1750FVI*  TheLightMeter;
    
    // Object to manage the configuration
    OPCConfigClass ConfigurationManager;
    
    // Configuration object
    camera_config_t TheConfiguration;
    
	// A flag which, when set, indicates that a capture is currently underway
	bool CaptureUnderway;
	
	// The number of the next file we can write. This number is turned into a
	// string and used to name the file.
	unsigned long NextFileNumber;
	
	// Figure out the name (number) of the next file we can write to
	void LoadNextFileNumber (void);
	
	// Save the number of the next file we can write so that we can use it on
	// power-up. This number is turned into a string and used to name the file.
	void SaveNextFileNumber (void);

	// Write the header at the start of an image file. Returns a flag which,
	// when set, indicates that the write was successful.
	bool WriteImageFileHeader(void);
	
	// Move the sensor to the first pixel position of the image
	void MoveSensorHome (void);
	
	// Handles for the files we use - could use one handle, but this makes
	// code a little more clear
	File ImageFile, NameFile;
	
	// Return a pointer to the name of the next image file to save
	char* GetImageFileName (void);
	
};

#endif
