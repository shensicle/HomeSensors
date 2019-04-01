#ifndef _OPCAPPDEFS_H
#define _OPCAPPDEFS_H

	typedef enum capture_mode_t
	{
		capture_mode_grayscale = 0x00,
		capture_mode_RGB       = 0x01
	};
	
	typedef enum bit_depth_t 
	{
		bit_depth_8        = 0x00,
		bit_depth_16       = 0x01
	};
	
	// Structure to hold the camera config. Used to store configuration in non-volatile
	// storage and to pass it back and forth betweent this application and the
	// user interfaces. *** PROBABLY SHOULD BE MOVED TO A DEFS.H FILE ***
	typedef struct camera_config_t
	{
		unsigned short version;
		unsigned short horizRes;
		unsigned short vertRes;
		capture_mode_t captureMode;
		bit_depth_t    bitDepth;
	};
	
#define CONFIGURATION_VERSION 1
#define DEFAULT_HORIZ_RES     300
#define DEFAULT_VERT_RES      300
#define DEFAULT_CAPTURE_MODE  capture_mode_t::capture_mode_grayscale
#define DEFAULT_BIT_DEPTH     bit_depth_t::bit_depth_8

#endif
