/* controlclass.h - include file for win32 gui controls classes */
#ifndef CONTROLCLASS_H
#define CONTROLCLASS_H

#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <string>

using namespace std;

class editbox
{
private:
	HWND hWnd;
	char* hLastGet;
public:
	editbox()
	{
		hWnd=0;
		hLastGet=0;
	}
	void SetHwnd(HWND h)
	{
		hWnd=h;
	}
	bool LoadHwndFromResource(HWND hOwner, int rsrcId)
	{
		hWnd=GetDlgItem(hOwner, rsrcId);
		if(!hWnd)
			return false;
		else
			return true;
	}
	void Disable()
	{
		EnableWindow(hWnd, FALSE);
	}
	void Enable()
	{
		EnableWindow(hWnd, TRUE);
	}
	LRESULT SendMsg(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		return SendMessage(hWnd, msg, wparam, lparam);
	}
	char* GetText();
	bool SetText(string& buffer);
	bool SetText(char* buffer);
	int GetTextLength();
	bool AppendText(string& inputbuffer);
	bool AppendText(char* inputbuffer);
	bool ScrollToEnd();
};

typedef struct sel_arr{
	int *iarr;
	int count;
}sell_arr;

class listbox{
public:
	HWND hWnd;
	void SetHwnd(HWND hw){hWnd=hw;}
	HWND GetHwnd()
	{
		return hWnd;
	}
	bool LoadHwndFromResource(HWND hOwner, int rsrcId)
	{
		hWnd=GetDlgItem(hOwner, rsrcId);
		if(!hWnd)
			return false;
		else
			return true;
	}
	string* GetSelections()
	{
		sel_arr sArr=GetSelections(0);
		string* s=new string[sArr.count];
		for(int i=0; i<sArr.count; i++)
		{
			s[i]=GetString(sArr.iarr[i]);
		}
		return s;
	}
	sel_arr GetSelections(int dummy)
	{
		sel_arr slr;
		int sel=SendMessage(hWnd, LB_GETSELCOUNT, 0, 0);
		//int* sel_list=(int*)malloc(sel*sizeof(int)+1);
		int* sel_list=new int[sel*sizeof(int)+1];
		SendMessage(hWnd, LB_GETSELITEMS, sel, (LPARAM)sel_list);
		slr.iarr=sel_list;
		slr.count=sel;
		return slr;
	}
	void AddString(string& s)
	{
		AddString((char*)s.c_str());
	}
	void AddString(char* s)
	{
		SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)s);
	}
	void DelString(int i)
	{
		SendMessage(hWnd, LB_SETSEL, FALSE, i);
		SendMessage(hWnd, LB_DELETESTRING, i, 0);
	}
	void SelectAll()
	{
		int i=GetCount();
		for(int j=0; j<=i; j++)
			SendMessage(hWnd, LB_SETSEL, TRUE, j);
	}
	void Clear()
	{
		int i=GetCount();
		for(; i>=0; i--)
			DelString(i);
	}
	void DeselectAll()
	{
		SendMessage(hWnd, LB_SETSEL, FALSE, -1);
	}
	int GetCount()
	{
		return SendMessage(hWnd, LB_GETCOUNT, 0, 0);
	}
	string GetString(int i, int expectedlength=80)
	{
		char* tmp=new char[expectedlength];
		GetString(i, tmp);
		string retStr=tmp;
		delete[] tmp;
		return retStr;
	}
	int GetString(int i, string& buffer)
	{
		return GetString(i, (char*)buffer.c_str());
	}
	int GetString(int i, char* buffer)
	{
		return SendMessage(hWnd, LB_GETTEXT, i, (LPARAM)buffer);
	}
	void RemoveSelections()
	{
		sel_arr s=GetSelections(0);
		for(int i=s.count; i>=0; i--)
		{
			this->DelString(s.iarr[i]);
		}
	}
};

class combobox{
private:
	char *text;
public:
	HWND hWnd;
	combobox()
	{
		hWnd=NULL;
		text=NULL;
	}
	void SetHwnd(HWND hw){hWnd=hw;}
	HWND GetHwnd()
	{
		return hWnd;
	}
	bool LoadHwndFromResource(HWND hOwner, int rsrcId)
	{
		hWnd=GetDlgItem(hOwner, rsrcId);
		if(!hWnd)
			return false;
		else
			return true;
	}
	void AddItem(char* item)
	{
		SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)item);
		return;
	}
	char* GetSelection()
	{
		int i=SendMessage(hWnd, CB_GETCURSEL, NULL, NULL);
		if(text)
			delete[] text;
		text=new char[SendMessage(hWnd, CB_GETLBTEXTLEN, i, NULL)+1];
		SendMessage(hWnd, CB_GETLBTEXT, i, (LPARAM)text);
		return text;
	}
	void SetSel(int i)
	{
		SendMessage(hWnd, CB_SETCURSEL, i, 0);
	}
};

#endif