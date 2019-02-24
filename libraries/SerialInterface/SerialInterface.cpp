/*
Copyright © 2011 Scott Henwood/shensicle photographic. All Rights Reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products 
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Scott Henwood/shensicle photographic "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <SerialInterface.h>


// Size of buffer for incoming serial characters
#define BUF_SIZE 80

	
// --------------------------------------------------------------------------- 
// Constructor
SerialInterface::SerialInterface (ApplicationBase* theApp, int numProfiles)
	: Parser(&InputString) 
{ 
	InputString = ""; 
	TheApp = theApp; 
	NumProfiles = numProfiles;
	CommandComplete = false; 
	
	// Reserve space for incoming commands
        InputString.reserve(BUF_SIZE);

}
	
// --------------------------------------------------------------------------- 
// Add a character to the command currently being built up
void SerialInterface::Add (char inChar)
{
	InputString += inChar;
	
	if (inChar == '\n')
		CommandComplete = true;
}
	
// --------------------------------------------------------------------------- 
// If we have a complete command, parse and act on it
void SerialInterface::Update (void)
{
	unsigned short curProfile, prgNumber;
	
	int status = 0;
	
	if (CommandComplete == true)
	{
		Parser.Reset();
		
		    char command = Parser.GetChar();
		    command = toupper(command);
		    
		    switch (command)
		    {
		    	    	// Run
		    		case 'R':
		    			TheApp->Run();
		    			break;
        
		    		// Stop
		    		case 'S':
		    			TheApp->Stop();
		    			break;
        
		    		// Set active profile
		    		case 'A':        
		    			curProfile = (unsigned short)Parser.GetUnsignedLong();
		    			curProfile --; // User works with profile numbers starting at one
		    				       // but application starts at 0
                           
		    			TheApp->SetCurrentProfile (curProfile);
		    			break;  
        
		    		// Set low power mode
		    		case 'L':        
		    			TheApp->SetLowPowerMode();
		    			break;  
        
		    		// Set normal power mode
      				case 'N':        
      					TheApp->SetNormalPowerMode();
      					break;  
        
      				// Profile mode
      				case 'P':
    
      					// Read the profile number from the input string and pass the rest of the string to
      					// the appropriate profile
      					prgNumber = (unsigned short)Parser.GetUnsignedLong();

      					prgNumber --;   // User deals with profile numbers starting at one
        
      					if (prgNumber < NumProfiles)
      					{
      						TheApp->Stop();
      						ProfileBase* theProg = TheApp->GetProfile(prgNumber);
      						status = theProg->SetConfig(&Parser);
      					}
      					else
      					{
      						// Invalid profile number
      						status = -1;
      					}
      					break;
        
      				// Get current configuration
      				case 'G':
      					TheApp->GetConfig();
      					break;
        
      				default:
      					status = -1;
      					break;

      		    }
    
      		    CommandComplete = false;
      		    InputString = ""; 
  
      		    // Turn status to a more human friendly value
      		    if (status == 0)
      		    	    Serial.println (F("+OK"));
      		    else
      		    	    Serial.println (F("-ERR"));
      	}  
}

