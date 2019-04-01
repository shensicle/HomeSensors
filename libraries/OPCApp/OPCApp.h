#ifndef _1PCAPP_H
#define _!PCAPP_H

#include "OPCAppDefs.h"

// One-pixel camera application

class OPCApp
{
public:
		
	
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
	
		                  
	
protected:
	
	// A flag which, when set, indicates that a capture is currently underway
	bool CaptureUnderway;
	

	
};

#endif
