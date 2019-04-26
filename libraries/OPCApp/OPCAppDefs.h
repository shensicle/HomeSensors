#ifndef _OPCAPPDEFS_H
#define _OPCAPPDEFS_H

	typedef enum bit_depth_t 
	{
		bit_depth_8        = 0x00,
		bit_depth_16       = 0x01
	};
	
	typedef enum colour_mode_t
	{
		colour_mode_grayscale = 0x00,
		colour_mode_RGB       = 0x01
	};
	
	typedef enum filter_method_t 
	{
		filter_method_default   = 0x00
	};
	
	typedef enum scan_t 
	{
		scan_progressive   = 0x00,
		scan_zigzag        = 0x01
	};
	
	
	// Structure to hold the camera config. Used to store configuration in non-volatile
	// storage and to pass it back and forth betweent this application and the
	// user interfaces. This structure is also stored in the raw image file so that
	// the PNG converter can understand the raw image.
	typedef struct camera_config_t
	{
		unsigned long   version;
		unsigned long   imageWidth;
		unsigned long   imageHeight;
		bit_depth_t     bitDepth;
		colour_mode_t   colourType;
		unsigned char   unused1;
		filter_method_t filterMethod;
		scan_t          scanType;
	};
	
#define OPC_CONFIGURATION_VERSION   1
#define DEFAULT_IMAGE_WIDTH       300
#define DEFAULT_IMAGE_HEIGHT      300
#define DEFAULT_BIT_DEPTH         bit_depth_t::bit_depth_16
#define DEFAULT_COLOUR_MODE       colour_mode_t::colour_mode_grayscale
#define DEFAULT_FILTER_METHOD     filter_method_t::filter_method_default
#define DEFAULT_SCAN_TYPE         scan_t::scan_zigzag

#endif


