
#include "crclist.h"
#include <string>
#include <algorithm>

using namespace std;

string crcList::getline(string& src, int lineindex)
{
	int i, j;
	string nomore="NOMORELINES";
	for(i=0, j=0; i<src.length() && j<lineindex; i++)
	{
		if(src[i]=='\n')
		{
			j++;
			continue;
		}
		if(!src[i])
			return nomore;
	}
	if(!src[i])
        return nomore;
	string cpy;
	for(j=0; (src[i] && (src[i]!='\n') && (src[i]!='\r')); i++, j++)
		cpy+=src[i];
	return cpy;
}

crcList::entry* crcList::parse(string& line)
{
    crcList::entry *e=new crcList::entry();
    int i;
    string crc;
    string name;
    for(i=0; line[i]==' ' || line[i]=='\t'; i++)
    {
        // skip white space
    }
    for(; line[i]!=' ' && line[i]!='\t'; i++)
    {
        crc+=line[i];
    }
    e->setcrc(atoi(crc.c_str()));
    // now we copy the file name
    int j=line.length();
    for(; line[j]==' ' || line[j]=='\t'; j--)
    {
        // skip white space at the end of the line
    }
    for(; line[i]==' ' || line[i]=='\t' || line[i]=='='; i++)
    {
        // skip white space and equals sign
    }
    for(; i!=j; i++)
    {
        name+=line[i];
    }
    e->setpath(name);
    return e;
}

bool crcList::load(char* data) throw (FormatInvalidException) 
{
	string s=data;
	return load(s);
}

bool crcList::load(string& data) throw (FormatInvalidException)
{
    string line=getline(data, 0);
    if(strcmpi(line.c_str(), STANDARD_CRCLIST_HEADER))
    {
        throw FormatInvalidException();
    }
    // otherwise, we will keep going and parse it into our list.
    if(!crclst.empty())
    {
        crclst.clear();
    }
    for(int i=1; ; ++i)
    {
        crcList::entry* e=new crcList::entry();
        line=getline(data, i);
        if(line=="NOMORELINES")
            break;
        crclst.push_back(*parse(line));
    }
}

bool crcList::addfile(file& newFile)
{
    crcList::entry e;
    crcAbleFile c;
    unsigned int crc;
    c.setpath(newFile.getpath());
    try{
        crc=c.getCRC();
    }catch(FileEmptyException e)
    {
        crc=0;
    }
    e.setcrc(crc);
    e.setpath(newFile.getpath());
    crclst.push_back(e);
    return true;
}

bool crcList::checkfile(crcAbleFile& chkFile)
{
	for(list<crcList::entry>::iterator it=crclst.begin(); it!=crclst.end(); ++it)
	{
		if(it->getpath()==chkFile.getpath())
		{
			return true;
		}
	}
	return false;
}

unsigned long crcList::getcrc(crcAbleFile& chkFile)
{
	for(list<crcList::entry>::iterator it=crclst.begin(); it!=crclst.end(); ++it)
	{
		if(it->getpath()==chkFile.getpath())
		{
			return it->getcrc();
		}
	}
	return 0;
}

bool crcList::updatefile(crcAbleFile& updFile) throw (FileNonExistentException)
{
    for(list<crcList::entry>::iterator it=crclst.begin(); it!=crclst.end(); ++it)
    {
        if(it->getpath()==updFile.getpath())
        {
            try{
                it->setcrc(updFile.getCRC());
            }catch(FileEmptyException e)
            {
                it->setcrc(0);
            }
            return true;
        }
    }
    return false;
}

bool crcList::deletefile(crcAbleFile& delFile) throw (FileNonExistentException)
{
    for(list<crcList::entry>::iterator it=crclst.begin(); it!=crclst.end(); ++it)
    {
        if(it->getpath()==delFile.getpath())
        {

            crclst.erase(it);
            return true;
        }
    }
    return false;
}

string crcList::compile() throw (exception)
{
    string outStr(STANDARD_CRCLIST_HEADER);
    outStr+="\n";
    char outNum[64];
    if(crclst.empty())
    {
        throw std::exception();
    }
    for(list<crcList::entry>::iterator it=crclst.begin();
        it!=crclst.end(); ++it)
    {
        //outStr+=it->getcrc();   // use sprintf
        sprintf(outNum, "%d", it->getcrc());
        outStr+=outNum;
        outStr+=" = ";
        outStr+=it->getpath();
        outStr+="\n";
    }
    return outStr;
}
