#include "global.h"
#include "..\..\file\file.h"
#include "setting.h"
#include "status.h"
#include <list>
#include "..\..\filelist\filelist.h"
#include "..\..\crclist\crclist.h"
#include "..\..\crclist\crcablefile.h"

#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#define STANDARD_CRCLIST_HEADER "<CRCLIST>"
#define STANDARD_FILELIST_HEADER "<FILELIST>"

using namespace std;

extern setting gSetting;
extern status botStatus;
extern char workingdir[MAX_PATH];

class fileProcessor
{
public:
	fileProcessor(){}
	string processFiles(list<crcAbleFile> include, list<crcAbleFile> exclude);
};

#endif