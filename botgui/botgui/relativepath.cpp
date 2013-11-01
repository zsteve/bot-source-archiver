#include <exception>
#include <string>
#include "relativepath.h"

using namespace std;

void stringToLowerCase(string &str)
{
    for(int i=0; i<str.size(); i++)
    {
        if(isupper(str[i])) str[i]=tolower(str[i]);
    }
    return;
}

int getLastSimilarDirIndex(string path1, string path2) throw (exception)
{
    string& shortestString=(path1.size()>path2.size()) ? path2 : path1;
    string& otherString=(shortestString==path1) ? path2 : path1;
    int j=0;
    for(int i=0; i<shortestString.size(); i++)
    {
        if(shortestString[i]!=otherString[i])
        {
            return j;
        }
        if(shortestString[i]=='\\' || shortestString[i]=='/')
        {
            j=i;
        }
    }
    return j;
}

int getDirLevels(string path)
{
    int levelsCount=0;
    for(int i=0; i<path.size(); i++)
    {
        if(path[i]=='/' || path[i]=='\\')
            levelsCount++;
    }
    return levelsCount;
}

string getFileName(string path)
{
    // returns the name of the file pointed to by
    // path
    string fname("");
    int i=0;
    for(i=path.size(); path[i]!='\\' && path[i]!='/'; i--){}
    if(i==path.size()) // this means the path points to no file
        return "";
    for(i++; i<path.size(); i++)
    {
        fname+=path[i];
    }
    return fname;
}

string getRelativePath(string path, string basePath)
{
    // returns the path of path relative to basePath
    string relativePath("");
    stringToLowerCase(path);
    stringToLowerCase(basePath);
	if(basePath[basePath.size()]!='\\')
		basePath+="\\";
    int lastSimilarDir=0;
    try{
        lastSimilarDir=getLastSimilarDirIndex(path, basePath);
    }catch(exception e){
        throw exception();       // paths are totally similar
    }
    int dirLevelsUp=(getDirLevels(basePath.substr(lastSimilarDir)));
    for(int i=0; i<dirLevelsUp-1; i++)
    {
        relativePath+="\\..";
    }
    relativePath+=path.substr(lastSimilarDir);
    return relativePath.substr(1, relativePath.size());
}