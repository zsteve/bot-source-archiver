#include <list>
#include <string>
#include "..\..\file\file.h"
#include "..\..\filelist\filelist.h"
#include "..\..\crclist\crclist.h"
#include "..\..\crclist\crcAbleFile.h"

#ifndef GLOBAL_H
#define GLOBAL_H

using namespace std;

extern list<crcAbleFile> includefiles, excludefiles;
extern file inifile, crclist, filelist;
extern char workingdir[MAX_PATH];

void loadIncExcList(list<crcAbleFile>& inc, list<crcAbleFile>& exc);
void saveIncExcList(list<crcAbleFile>& inc, list<crcAbleFile>& exc);
void loadLists(fileList& fl, crcList& cl);
void saveLists(fileList& fl, crcList& cl);

#endif