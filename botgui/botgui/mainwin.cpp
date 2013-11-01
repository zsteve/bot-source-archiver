/* mainwin.cpp - source file for main window function */
/* standard includes */
#include <windows.h>
#include <commctrl.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "..\..\file\file.h"
#include "mainwin.h"
#include "setting.h"
#include "tabpage.h"
#include "rsrc\resource.h"
#include "controlclass.h"
#include "..\..\filelist\filelist.h"
#include "..\..\crclist\crclist.h"
#include "fileprocessor.h"
#include "status.h"
#include "..\..\ftpwrapper\ftpwrapper\ftplib.hpp"

extern status botStatus;
extern setting gSetting;
extern char workingdir[MAX_PATH];
extern const char botdir[MAX_PATH]="\\bot";
extern file inifile, crclist, filelist;

HWND hTabControl;
HWND hCurrentTab;

bool loadSettings()
{
	// attempts to load from workingdir
	string wDir=workingdir;
	inifile.setpath(wDir+"\\bot.ini");
	crclist.setpath(wDir+"\\crclist.txt");
	filelist.setpath(wDir+"\\filelist.txt");
	if(inifile.exists())
	{
		gSetting.deserialize((char*)inifile.load());
	}else{
		MessageBox(NULL, "Error : bot.ini not found", "Error", MB_ICONERROR);
	}
	return true;
}

bool uploadFileToServer(string path)
{
	// uploads the input file 
	// the server specified in gSetting.
	ftpconn* conn;
	try{
		conn=new ftpconn(gSetting.ftp.addr, gSetting.ftp.user, gSetting.ftp.pass);
	}catch(ConnectionFail e){
		MessageBox(NULL, "Error : Connection failed to ftp server", "Error", MB_ICONERROR);
		return false;
	}
	string srcpath=workingdir;
	srcpath+="\\";
	srcpath+=path;
	return conn->sendfile(gSetting.ftp.path+path, srcpath, FTPLIB_IMAGE);
}

DWORD WINAPI botThread(LPVOID param)
{
	fileProcessor fp;
	string outFileName=fp.processFiles(includefiles, excludefiles);
	if(outFileName!="")
	{
		MessageBox(NULL, "Press OK once the compression program has completed", "Message", MB_OK);
		botStatus.setCurrentStatus("Uploading file to server...");
		uploadFileToServer(outFileName);
		botStatus.setCurrentStatus("Completed!");
	}
	return 0;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static editbox statusBox;
    switch(uMsg)
    {
        case WM_INITDIALOG:
			statusBox.LoadHwndFromResource(hWnd, IDC_STATUS);
			statusBox.SetText(botStatus.getCurrentStatus());
			{
				file iniFile;
				string iniPath=workingdir;
				iniPath+="\\bot.ini";
				iniFile.setpath(iniPath);
				if(iniFile.exists())
				{
					gSetting.deserialize((char*)iniFile.load());
				}else{
					// ... TODO
				}
			}
			{
				loadIncExcList(includefiles, excludefiles);
			}
			/* create tab control, etc */
			INITCOMMONCONTROLSEX ix;
			ix.dwSize = sizeof(INITCOMMONCONTROLSEX);
			ix.dwICC = ICC_TAB_CLASSES;
			InitCommonControlsEx(&ix);
			hTabControl = GetDlgItem(hWnd,IDC_TABMAIN);
			TCITEM ti;
			ti.mask = TCIF_TEXT;
			ti.pszText = "Include files";
			TabCtrl_InsertItem(hTabControl,0,&ti);
			ti.pszText = "Exclude files";
			TabCtrl_InsertItem(hTabControl,1,&ti);
			ti.pszText = "Settings";
			TabCtrl_InsertItem(hTabControl,2,&ti);
			TabCtrl_SetCurSel(hTabControl,0);
			hCurrentTab = CreateDialog(hInst,MAKEINTRESOURCE(IDC_TAB1),hTabControl,&TabProc1); // Setting dialog to tab one by
			SetTimer(hWnd, 1, 100, NULL);           
			return TRUE;
        case WM_SIZE:

            break;
		case WM_TIMER:
			statusBox.SetText(botStatus.getCurrentStatus());
            break;
		case WM_NOTIFY:
		{
		  switch(((LPNMHDR)lParam)->code)
		  {
			case TCN_SELCHANGE: // message sent because someone changed the tab selection (clicked on another tab)
			{
				loadSettings();
				SendMessage(hCurrentTab, WM_COMMAND, IDOK, NULL);
				EndDialog(hCurrentTab,0); // we don't want the current tab, kill it
				switch(TabCtrl_GetCurFocus(hTabControl))
				{
				case 0:
					hCurrentTab = CreateDialog(hInst,MAKEINTRESOURCE(IDC_TAB1),hTabControl,&TabProc1);
					break;
				case 1:
					hCurrentTab = CreateDialog(hInst,MAKEINTRESOURCE(IDC_TAB2),hTabControl,&TabProc2);
					break;
				case 2:
					hCurrentTab = CreateDialog(hInst,MAKEINTRESOURCE(IDC_TAB3),hTabControl,&TabProc3);
				default:
					break;
				}
				return TRUE;
			}
		  }
		  return TRUE;
		}
        case WM_CLOSE:
            EndDialog(hWnd, 0);
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
			/* add control functions here */
			case IDOK:
				// OK button was clicked; we must start processing
				{
					SendMessage(hCurrentTab, WM_COMMAND, IDOK, NULL);
					saveIncExcList(includefiles, excludefiles);
					CreateThread(NULL, NULL, botThread, hWnd, NULL, NULL);
				}
				break;
			case ID_FILE_NEWPROJECT:
				// new project

				break;
			case ID_FILE_LOADPROJECT:
				// open project
				{
					fileProcessor fileProcess;
					fileProcess.processFiles(includefiles, excludefiles);
				}
				break;
			case ID_FILE_QUIT:
				SendMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			case ID_ABOUT_ABOUTBOT:
				MessageBox(hWnd, "bot v0.2alpha - (c) zsteve", "info", MB_OK);
				break;
            }
    }

    return FALSE;
}