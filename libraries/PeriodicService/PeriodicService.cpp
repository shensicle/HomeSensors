#include "PeriodicService.h"

// ----------------------------------------------------------------------------
// Constructor - set initial intervals
PeriodicService::PeriodicService (unsigned long serviceInterval, unsigned long updateInterval)
{
	if (serviceInterval <= 0)
		ServiceInterval = 100;
	else
	{
		ServiceInterval = serviceInterval;
		SetUpdateInterval (updateInterval);
	}
}
	
// ----------------------------------------------------------------------------
// Service this object. The return value indicates whether or not Update() was called
bool PeriodicService::Service (void)
{
	bool returnValue = false;
	
	TimeToGo -= ServiceInterval;
	
	if (TimeToGo <= 0)
	{
		returnValue = true;
		TimeToGo = UpdateInterval;
		Update();
	}
	return (returnValue);
}
	
// ----------------------------------------------------------------------------
// Change the Update interval
void PeriodicService::SetUpdateInterval (unsigned long newInterval)
{	
	UpdateInterval = newInterval;	
	TimeToGo = UpdateInterval;
}
