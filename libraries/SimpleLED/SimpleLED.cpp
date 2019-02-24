/*
Copyright 2011 Scott Henwood/shensicle photographic. All Rights Reserved.

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

#include <SimpleLED.h>
	
// --------------------------------------------------------------
SimpleLED::SimpleLED (	byte outputPin,
						bool isActiveHigh,
						unsigned long serviceInterval, // milliseconds
						unsigned long updateInterval)  // milliseconds
: TwoStateOutput (outputPin, isActiveHigh), PeriodicService (serviceInterval, updateInterval)
{
	CurrentState = OFF;
	FlashMode = false;
}

	
// --------------------------------------------------------------
// Method to enable/disable flashing
void SimpleLED::SetFlashMode (bool flashEnabled)
{
	
	FlashMode = flashEnabled;
	SetOff();
}


// --------------------------------------------------------------
// Method called by the controlling program at UpdateInterval to allow
// us to manage the LED. Returns true if we are still running after
// the call and false otherwise.
void SimpleLED::Update (void)
{
	// If we are flashing
	if (FlashMode == true)
	{
		if (CurrentState == ON)
		{
			SetOff();
		}
		else
		{
			SetOn();
		}			
	}
}

// --------------------------------------------------------------
void SimpleLED::SetOn (void)
{
	TwoStateOutput::SetOn();
	CurrentState = ON;
}

// --------------------------------------------------------------
void SimpleLED::SetOff (void)
{
	TwoStateOutput::SetOff();
	CurrentState = OFF;
}


