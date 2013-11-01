/* tabpage.cpp - tab pages dlgprocs */
#include <windows.h>
#include <string>
#include "tabpage.h"
#include "rsrc\resource.h"
#include "global.h"
#include "setting.h"
#include "controlclass.h"
#include "..\..\filelist\filelist.h"
#include "..\..\file\file.h"
#include "..\..\crclist\crcablefile.h"
#include "relativepath.h"

using namespace std;

/* tab dlgprocs */
extern setting gSetting;
extern list<crcAbleFile> includefiles, excludefiles;
extern char workingdir[MAX_PATH];

BOOL CALLBACK TabProc1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static listbox listMain;
    switch(uMsg)
    {
        case WM_INITDIALOG:
			listMain.LoadHwndFromResource(hWnd, IDC_LIST1);
			{
				if(!includefiles.empty())
				{
					for(list<crcAbleFile>::iterator it=includefiles.begin();
						it!=includefiles.end(); ++it)
					{
						listMain.AddString(it->getpath());
					}
				}
			}
			return TRUE;
        case WM_CLOSE:
			SendMessage(hWnd, WM_COMMAND, IDOK, 0);
            EndDialog(hWnd, 0);
            return TRUE;
		case WM_DROPFILES:
			if(gSetting.isLoaded)
			{
				{
					int* dropcount=new int;
					*dropcount=0;
					*dropcount=DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, NULL);
					if(dropcount>0)
					{
						char *dropnames=new char[MAX_PATH];
						for(int i=0; i<*dropcount; i++)
						{
							DragQueryFile((HDROP)wParam, i, dropnames, MAX_PATH);
							listMain.AddString(getRelativePath(dropnames, workingdir));
						}
						delete[] dropnames;
					}
					delete dropcount;
				}
			}else{
				MessageBox(hWnd, "Error : .ini file not loaded", "Error", MB_ICONASTERISK);
			}
			break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
			case IDCANCEL:
				break;
			case IDC_SELALL:
				listMain.SelectAll();
				break;
			case IDC_SELNONE:
				listMain.DeselectAll();
				break;
			case IDC_REMOVESEL:
				listMain.RemoveSelections();
				break;
			case IDOK:
				if(!includefiles.empty())
				{
					includefiles.clear();
				}
				{
					int count=listMain.GetCount();
					for(int i=0; i<count; i++)
					{
						crcAbleFile f;
						f.setpath(listMain.GetString(i, MAX_PATH));
						includefiles.push_back(f);
					}
				}
				break;
            }
    }
    return FALSE;
}

BOOL CALLBACK TabProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static listbox listMain;
    switch(uMsg)
    {
        case WM_INITDIALOG:
			listMain.LoadHwndFromResource(hWnd, IDC_LIST1);
			{
				if(!excludefiles.empty())
				{
					for(list<crcAbleFile>::iterator it=excludefiles.begin();
						it!=excludefiles.end(); ++it)
					{
						listMain.AddString(it->getpath());
					}
				}
			}
			return TRUE;
		case WM_DROPFILES:
			if(gSetting.isLoaded)
			{
				{
					int* dropcount=new int;
					*dropcount=0;
					*dropcount=DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, NULL);
					if(dropcount>0)
					{
						char *dropnames=new char[MAX_PATH];
						for(int i=0; i<*dropcount; i++)
						{
							DragQueryFile((HDROP)wParam, i, dropnames, MAX_PATH);
							listMain.AddString(getRelativePath(dropnames, workingdir));
						}
						delete[] dropnames;
					}
					delete dropcount;
				}
			}else{
				MessageBox(hWnd, "Please create or load a project first", "Error", MB_ICONASTERISK);
			}
			break;
        case WM_CLOSE:
			SendMessage(hWnd, WM_COMMAND, IDOK,0);
            EndDialog(hWnd, 0);
            return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
			case IDC_SELALL:
				listMain.SelectAll();
				break;
			case IDC_SELNONE:
				listMain.DeselectAll();
				break;
			case IDC_REMOVESEL:
				listMain.RemoveSelections();
				break;
			case IDOK:
				if(!excludefiles.empty())
				{
					excludefiles.clear();
				}
				{
					int count=listMain.GetCount();
					for(int i=0; i<count; i++)
					{
						crcAbleFile f;
						f.setpath(listMain.GetString(i, MAX_PATH));
						excludefiles.push_back(f);
					}
				}
				break;
            }
    }
    return FALSE;
}

BOOL CALLBACK TabProc3(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static editbox ftpaddr, ftppass, ftpuser, ftppath, filename, fileversion;
    switch(uMsg)
    {
        case WM_INITDIALOG:
			ftpaddr.LoadHwndFromResource(hWnd, IDC_FTPADDRESS);
			ftpuser.LoadHwndFromResource(hWnd, IDC_FTPUSER);
			ftppass.LoadHwndFromResource(hWnd, IDC_FTPPASS);
			ftppath.LoadHwndFromResource(hWnd, IDC_FTPPATH);
			filename.LoadHwndFromResource(hWnd, IDC_FILENAME);
			fileversion.LoadHwndFromResource(hWnd, IDC_VERSION);
			ftpaddr.SetText(gSetting.ftp.addr);
			ftpuser.SetText(gSetting.ftp.user);
			ftppass.SetText(gSetting.ftp.pass);
			ftppath.SetText(gSetting.ftp.path);
			filename.SetText(gSetting.file.filename);
			{
				char* tempNum=new char[64];
				sprintf(tempNum, "%i", gSetting.file.version);
				string tempNumStr=tempNum;
				fileversion.SetText(tempNum);
				delete[] tempNum;
			}
			if(gSetting.option.incremental)
			{
				gSetting.option.incremental=0;	// we're doing this for lack of a better way
												// to keep gSetting..incremental's value
												// after SendDlgItemMessage
												// so it will be TRUE after clicking
				SendDlgItemMessage(hWnd, IDC_MAKEINCREMENTAL, WM_LBUTTONDOWN, 0, 0); 
				SendDlgItemMessage(hWnd, IDC_MAKEINCREMENTAL, WM_LBUTTONUP, 0, 0); 
			}
			return TRUE;
        case WM_CLOSE:
			SendMessage(hWnd, WM_COMMAND, IDOK, 0);
            EndDialog(hWnd, 0);
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
			/* add control functions here */
			case IDOK:
				{
					gSetting.ftp.addr=ftpaddr.GetText();
					gSetting.ftp.pass=ftppass.GetText();
					gSetting.ftp.user=ftpuser.GetText();
					gSetting.ftp.path=ftppath.GetText();
					gSetting.file.filename=filename.GetText();
					gSetting.file.version=atoi(fileversion.GetText());
					{
						string outStr=gSetting.serialize();
						{
							file outFile;
							string iniPath=workingdir;
							iniPath+="\\bot.ini";
							outFile.setpath(iniPath);
							outFile.write((char*)outStr.c_str());
						}
					}
				}
				break;
			case IDC_MAKEINCREMENTAL:
				// disabled because of some problems with getting the 
				// checkbox to work
				// gSetting.option.incremental=!gSetting.option.incremental;
				break;
			case IDC_FTPUPLOAD:
				break;
            }
    }
    return FALSE;
}