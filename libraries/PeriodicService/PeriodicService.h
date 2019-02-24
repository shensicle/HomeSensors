#ifndef _PERIODIC_SERVICE_H
#define _PERIODIC_SERVICE_H

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

// Base class for an object that is updated (serviced) periodically by a sketch
// at a consistent interval
class PeriodicService
{
protected:
	// How often Service() is called, in milliseconds
	unsigned long ServiceInterval;
	
	// How often to do something - must be >= ServiceInterval
	unsigned long UpdateInterval;
	
	// Time until we need to do the next thing
	unsigned long TimeToGo;
	
	// Called by Service() when it's time to do something
	virtual void Update (void) = 0;
	
	
public:
	// Constructor - set initial intervals
	PeriodicService (unsigned long serviceInterval, unsigned long updateInterval = -1);
	
	// Service this object. The return value indicates whether or not Update() was called
	bool Service (void);
	
	// Change the Update interval
	void SetUpdateInterval (unsigned long newInterval);
	
};

#endif

