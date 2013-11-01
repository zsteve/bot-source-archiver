/* controlclass.cpp - source file for win32 gui controls classes */

#include "controlclass.h"
#include <string>

using namespace std;

char* editbox::GetText()
{
	char *hGet;
	int l=GetWindowTextLength(hWnd);
	if(hLastGet)
		//free(hLastGet);
		delete[] hLastGet;
	//hGet=hLastGet=(char*)malloc(l+1);
	hGet=hLastGet=new char[l+1];
	GetWindowText(hWnd, hGet, l+1);
	return hGet;
}

bool editbox::SetText(string& buffer)
{
	char* b=(char*)buffer.c_str();
	return SetText(b);
}

bool editbox::SetText(char* buffer)
{
	SetWindowText(hWnd, buffer);
	return TRUE;
}

int editbox::GetTextLength()
{
	return GetWindowTextLength(hWnd);
}

bool editbox::AppendText(string& inputbuffer)
{
	char* i=(char*)inputbuffer.c_str();
	return AppendText(i);
}

bool editbox::AppendText(char* inputbuffer)
{
	char *hNew;
	int l=GetWindowTextLength(hWnd);
	//hNew= (char*) malloc(l+strlen(inputbuffer)+1);
	hNew=new char[l+strlen(inputbuffer)+1];
	GetWindowText(hWnd, hNew, l+strlen(inputbuffer)+1);
	strcat(hNew, inputbuffer);
	SetWindowText(hWnd, hNew);
	//free(hNew);
	delete[] hNew;
	return TRUE;
}

bool editbox::ScrollToEnd()
{
    SendMessage(hWnd, EM_SETSEL, GetWindowTextLength(hWnd)-2, GetWindowTextLength(hWnd));
    SendMessage(hWnd, EM_SCROLLCARET, 0, 0);
	return true;
}
