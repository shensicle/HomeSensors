#include "OPCApp.h"

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
	
	// Ensure sensor is in the right place
	
	// Open the output file
	
	// Start the capture thread
}
	
// -----------------------------------------------------------------------------	
// Abort a capture in progress
void OPCApp::AbortCapture (void)
{
	// Only do something if there is currently a capture underway
	
	// Signal the capture thread to stop and wait until it does
	
	// Delete the output file from the aborted capture
}

// -----------------------------------------------------------------------------	
void OPCApp::CaptureTask (void)
{
	// For each row in the image
	
		// For each column in the current row
		
		// Move sensor
		
		// Read sensor
		
		// Save sensor value
		
		// Check to make sure an abort hasn't been requested
}
	
