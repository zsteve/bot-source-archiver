#include "filelist.h"

string fileList::getline(string& src, int lineindex)
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

bool fileList::load(char* data) throw (FormatInvalidException)
{
	string s=data;
	return load(s);
}
bool fileList::load(string& data) throw (FormatInvalidException)
{
    if(getline(data, 0)!=STANDARD_FILELIST_HEADER)
    {
        throw FormatInvalidException();
    }
    // otherwise, each line in data is a file path.
    if(!filelst.empty())
    {
        filelst.clear();
    }
    fileList::entry e;
    for(int i=1; ; i++)
    {
        if(getline(data, i)=="NOMORELINES")
        {
            break;
        }
        e.setpath(getline(data, i));
        filelst.push_back(e);
    }
    return true;
}

bool fileList::addfile(file& newFile)
{
    fileList::entry e;
    e.setpath(newFile.getpath());
    filelst.push_back(e);
    return true;
}

bool fileList::checkfile(file& chkFile)
{
	fileList::entry e;
	e.setpath(chkFile.getpath());
	for(list<fileList::entry>::iterator it=filelst.begin();
		it!=filelst.end(); ++it)
	{
		if(it->getpath()==chkFile.getpath())
		{
			return true;
		}
	}
	return false;
}

bool fileList::updatefile(file& oldFile, file& updFile)
{
    for(list<fileList::entry>::iterator it=filelst.begin();
    it!=filelst.end(); ++it)
    {
        if(it->getpath()==oldFile.getpath())
        {
            it->setpath(updFile.getpath());
            return true;
        }
    }
    return false;
}

bool fileList::removefile(file& delFile)
{
    for(list<fileList::entry>::iterator it=filelst.begin();
    it!=filelst.end(); ++it)
    {
        if(it->getpath()==delFile.getpath())
        {
            filelst.erase(it);
            return true;
        }
    }
    return false;
}

string fileList::compile()
{
    string outStr(STANDARD_FILELIST_HEADER);
    outStr+="\n";
    for(list<fileList::entry>::iterator it=filelst.begin();
    it!=filelst.end(); ++it)
    {
        outStr+=it->getpath();
        outStr+="\n";
    }
    return outStr;
}
