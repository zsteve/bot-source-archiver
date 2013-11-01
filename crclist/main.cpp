#include <iostream>
#include "..\errorlog\log.h"
#include "crcablefile.h"
#include "crclist.h"
#include "../exceptions/exception.h"

using namespace std;

error_log gError;

string dat="<CRCLIST>\n123531 = C:\\fasdf\\asdfafsd\\asdfadf.bin\n23523 = C:\\fasdf\\asdfafsd\\asdfadf.bin\n157568356 = C:\\fasdf\\asdfafsd\\asdfadf.bin\n123531 = C:\\fasdf\\asdfafsd\\asdfadf.bin";

int main()
{
    crcList l;
    try{
        l.load(dat);
    }catch(FormatInvalidException a)
    {
        cout << "FormatInvalidException" << endl;
        return 0;
    }
    crcAbleFile c;
    try{
        c.setpath("D:\\test\\test");
        cout << c.getCRC() << endl;
    }catch(FileEmptyException e)
    {
        cout << "FileEmptyException" << endl;
    }catch(FileNonExistentException e)
    {
        cout << "FileEmptyException" << endl;
    }
    file* f=&c;
    l.addfile(*f);
    l.updatefile(c);
    l.deletefile(c);
    list<crcList::entry> k=l.getlist();
    for(list<crcList::entry>::iterator it=k.begin(); it!=k.end(); ++it)
    {
        cout << "Path: \t" << it->getpath() << endl;
        cout << "CRC: \t" << it->getcrc() << endl;
    }
    cout << l.compile() << endl;
    return 0;
}
