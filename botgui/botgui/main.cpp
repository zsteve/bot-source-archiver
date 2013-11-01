/* main.cpp for bot - local source code manager */

/* standard includes */
#include <windows.h>
#include <stdlib.h>
#include <string.h>

/* non-standard includes */
#include "main.h"
#include "global.h"
#include "rsrc\resource.h"
#include "controlclass.h"
#include "mainwin.h"
#include "setting.h"
#include "status.h"

/* global data */
char workingdir[MAX_PATH]={0};
setting gSetting;
list<crcAbleFile> includefiles, excludefiles;
file inifile, crclist, filelist;
status botStatus;

/* WinMain */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;
	GetCurrentDirectory(MAX_PATH, workingdir);
    // The user interface is a modal dialog box
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)DlgProc);
}