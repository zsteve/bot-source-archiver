#include <iostream>
#include "file.h"
#include "..\errorlog\log.h"

using namespace std;

extern error_log gError;
char* pMemory;
char* LoadFile(HWND hWnd, LPCTSTR fName)
{
	// loads a file into memory and returns a handle
	// to the memory block
	HANDLE hFile;
	int fSize=0;
	hFile=CreateFile(fName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		DWORD read;
		fSize=GetFileSize(hFile, NULL);
		pMemory=(char*)GlobalAlloc(GPTR, fSize+1);
		if (pMemory!=NULL)
			{
			if(ReadFile(hFile, pMemory, fSize, &read, NULL)!=0)
			{
				CloseHandle(hFile);
				return pMemory;
			}
			// otherwise .....
			CloseHandle(hFile);
			return NULL;
		}
	}
	return NULL;
}

int SaveFile(HWND hWnd, char* hBuffer, char* fName, int fSize)
	// writes a block of memory to a file.
{

	HANDLE hFile=CreateFile(fName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		DWORD written;
		if(WriteFile(hFile, hBuffer, fSize, &written, NULL)!=0)
		{
			CloseHandle(hFile);
			return 1;
		}
	CloseHandle(hFile);
	return -1;
	}
	CloseHandle(hFile);
	return -1;
}

int main()
{
    gError.setverbose(true);
    /*directory d;
    d.setpath("C:\\");
    d.listcontents();
    list<file>& c=d.contents;
    for(list<file>::iterator it=c.begin(); it!=c.end(); ++it)
    {
        cout << (*it).getpath() <<endl;
    }
    cout << c.size() << endl;*/
    file f;
    f.setpath("C:\\ReadmeDOS.txt");
    //cout << LoadFile(NULL, "C:\\ReadmeDOS.txt") << endl;            // this one uses the old C function
    SaveFile(NULL, "this is the old function", "C:\\ReadmeDOS.txt", 10);        // this one also fails for some reason
    f.write("trying to write");                                     // this fails and logs an error message
    return 0;
}
