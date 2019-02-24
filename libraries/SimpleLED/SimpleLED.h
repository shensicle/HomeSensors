#ifndef _SIMPLE_LED_H
#define _SIMPLE_LED_H

/*
Copyright 2019 Scott Henwood/shensicle photographic. All Rights Reserved.

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

#include <TwoStateOutput.h>

#include "PeriodicService.h"

class SimpleLED : public TwoStateOutput, public PeriodicService
{
private:
		
	typedef enum {OFF, ON} led_state_type;
	led_state_type CurrentState;
	
	// A flag which, when set, indicates we are flashing
	bool FlashMode;
	
	// Method called by the controlling program at UpdateInterval to allow
	// us to manage the LED. Returns true if we are still running after
	// the call and false otherwise.
	virtual void Update (void);
	
	
public:
	SimpleLED (byte outputPin,
		bool isActiveHigh,
		unsigned long serviceInterval,       // milliseconds
		unsigned long updateInterval = 300);
	
	
	// Method to enable/disable flashing
	void SetFlashMode (bool flashEnabled);
	
	// Change LED state
	virtual void SetOn (void);
	virtual void SetOff (void);
};


#endif
