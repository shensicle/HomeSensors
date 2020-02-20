#include "OPCLCD.h"

// ---------------------------------------------------------------------------
// Constructor - have to do everything in Begin(), which 
// must be called after Wire (I2C) is started
OPCLCD::OPCLCD ()
{
}

// ---------------------------------------------------------------------------
bool OPCLCD::Begin (void)
{
  bool returnValue = false;
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(Display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
      returnValue = true;
      Display.clearDisplay();
  }
  
  return (returnValue);
	
}

// ---------------------------------------------------------------------------
void OPCLCD::DisplayTextLine (char* theText,
	                          int   textSize,
	                          int   textColour,
	                          int   x,
	                          int   y)
{
    Display.setTextSize(textSize);          
    Display.setTextColor(textColour);
    Display.setCursor(x,y);  
    
    // Clear text area
    Display.fillRect(x, y, textSize*10,LCD_SCREEN_WIDTH,BLACK);
    Display.println (theText); 
    Display.display();
}

// ---------------------------------------------------------------------------
// Banner text goes at the top of the display
void OPCLCD::DisplayBannerLine (char* theTitle)
{
	DisplayTextLine (theTitle, BannerTextSize, WHITE, BannerOriginX, BannerOriginY);
}
    
// ---------------------------------------------------------------------------
// Header text introduces the status
void OPCLCD::DisplayHeadingLine (char* theHeading)
{
	DisplayTextLine (theHeading, HeaderTextSize, WHITE, HeaderOriginX, HeaderOriginY);
}
    
// ---------------------------------------------------------------------------
// What's currently going on
void OPCLCD::DisplayStatusLine (char* theStatus)
{
	DisplayTextLine (theStatus, StatusTextSize, WHITE, StatusOriginX, StatusOriginY);
}

// ---------------------------------------------------------------------------
void OPCLCD::DisplayStatusPercent (int statusVal, char* suffix)
{
	static char displayStr[14];
	
	sprintf(displayStr,"%d  %c",statusVal, suffix[0]);
	DisplayStatusLine (displayStr);
}

