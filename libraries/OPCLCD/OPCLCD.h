#ifndef _OPCLCD_H
#define _OPCLCD_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Physical device
#define LCD_SCREEN_WIDTH 128 // OLED display width, in pixels
#define LCD_SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// One-pixel camera LCD manager

class OPCLCD : protected Adafruit_SSD1306
{
public:
	
    OPCLCD ()
            : Display (LCD_SCREEN_WIDTH, LCDSCREEN_HEIGHT, &Wire, OLED_RESET),
            BannerOriginX  (0),
            BannerOriginY (10),
            HeaderOriginX  (0),
            HeaderOriginY (30),
            StatusOriginX  (0),
            StatusOriginY (40),
            BannerTextSize (2),
            HeaderTextSize (1),
            StatusTextSize (2);
    
    bool DisplayIsInitialized (void) { return DisplayInitialized; }
    
    // Banner text goes at the top of the display
    void DisplayBannerLine (char* theTitle);
    
    // Header text introduces the status
    void DisplayHeadingLine (char* theHeading = "Status:");
    
    // What's currently going on
    void DisplayStatusLine (char* theStatus); 
    void DisplayStatusPercent (int statusVal, char* suffix = " %");
	
	
protected:

    Adafruit_SSD1306 Display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    
    int BannerOriginX, BannerOriginY, HeaderOriginX, HeaderOriginY, StatusOriginX, StatusOriginY,
        BannerTextSize, HeaderTextSize, StatusTextSize;
    
    // A flag which, when set, indicates that the LCD hardware has initialized successfully
    bool IsInitialized = false;
    
    void ClearStatusArea();
    	
};

#endif
