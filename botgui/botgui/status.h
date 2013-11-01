#include <windows.h>
#include "rsrc\resource.h"
#include <string>

#ifndef STATUS_H
#define STATUS_H

using namespace std;

class status{
public:
	status()
	{
		currentStatus="ready";
	}
	void setCurrentStatus(string status)
	{
		currentStatus=status;
	}
	string getCurrentStatus()
	{
		return currentStatus;
	}
private:
	string currentStatus;
};

#endif