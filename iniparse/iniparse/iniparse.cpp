#include "iniparse.h"
#include <string>
#include <cstring>
#include <list>
#include <iostream>
#include <sstream>


enum{NOMORELINES};

iniline* iniparser::getdata()
{
	return &(*listIter);
}

iniline iniparser::Parse(string& tline)
{
	int i=0;
	string name, data;
	iniline* l=new iniline;
	l->IsNode=false;
	for(int i=0; (tline[i]==' ') || (tline[i]=='\t'); i++)
		if(!tline[i]){break;}
	for(int j=0; ((tline[i]!=' ') && (tline[i]!='\t') && (tline[i]!='\t') && (tline[i]!='\r') && (tline[i]!='\n') && tline[i]) && (tline[i]!='='); i++, j++)
	{
		name+=tline[i];
	}
	if(name[0]=='[' && name[name.length()-1]==']')
	{
		l->IsNode=true;
		strcpy(l->data.node.name, name.c_str());
		return *l;
	}
	// otherwise, it is either a sub or a node.
	for(; ((tline[i]==' ') || (tline[i]=='\t') || (tline[i]=='=')); i++); // skip until we get to the sub data
	for(int j=0; (((tline[i]!=' ') || (tline[i]!='\r') && (tline[i]!='\n')) && tline[i]); j++)
	{
		data+=tline[i++];
	}
	strcpy(l->data.sub.name, name.c_str());
	strcpy(l->data.sub.value, data.c_str());
	return *l;
}

string iniparser::GetLine(string& src, int lineindex)
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
	for(j=0; (src[i] && (src[i]!='\n')); i++, j++)
		cpy+=src[i];
	return cpy;
}

bool iniparser::Load(string& src)
{
	/* Load() Loads an Ini file from a string&*/
	iniline l;
	string c;
    if(!listob.empty())
        listob.clear();
    for(int i=0;;i++)
    {
        iniline *n;
        string t=GetLine(src, i);
        if(t=="NOMORELINES")
            break;
        c=t;
        l=Parse(c);
        listob.push_back(l);
    }
    return true;
}

bool iniparser::SelectNode(char* nodename)
{
    string s=nodename;
    return SelectNode(s);
}

bool iniparser::SelectNode(string& nodename)
{
	if(listob.empty())
        return false;
    for(list<iniline>::iterator it=listob.begin();
        it!=listob.end(); ++it)
    {
        if(it->data.node.name==nodename)
        {
            listIter=it;
            return true;
        }
    }
    return false;
}

bool iniparser::DoesNodeExist(char* nodename)
{
    string s=nodename;
    return DoesNodeExist(s);
}

bool iniparser::DoesNodeExist(string& nodename)
{
	if(listob.empty())
        return false;
    for(list<iniline>::iterator it=listob.begin();
        it!=listob.end(); ++it)
    {
        if(it->data.node.name==nodename)
        {
            return true;
        }
    }
    return false;
}

bool iniparser::GetSub(char* nodename, char* subname)
{
    string s=nodename;
    string t=subname;
    return GetSub(s, t);
}

bool iniparser::GetSub(string& nodename, string& subname)
{
	if(listob.empty())
        return false;
    list<iniline>::iterator it=listob.begin();
    for(; it->IsNode && it->data.node.name==nodename; ++it)
    {
        if(it==listob.end())
        {
            return false;
        }
    } // skip until it is found
    for(; !it->IsNode && it!=listob.end(); ++it)
    {
        if(it->data.sub.name==subname)
        {
            listIter=it;
            return true;
        }
    }
    return false;
}

bool iniparser::DoesSubExist(char* nodename, char* subname)
{
    string s=nodename;
    string t=subname;
    return DoesSubExist(s, t);
}

bool iniparser::DoesSubExist(string& nodename, string& subname)
{
	if(listob.empty())
        return false;
    list<iniline>::iterator it=listob.begin();
    for(; it->IsNode && it->data.node.name==nodename; ++it)
    {
        if(it==listob.end())
        {
            return false;
        }
    } // skip until it is found
    for(; !it->IsNode && it!=listob.end(); ++it)
    {
        if(it->data.sub.name==subname)
        {
            return true;
        }
    }
    return false;
}

bool iniparser::SelectSub(char* nodename, char* subname)
{
    string s=nodename;
    string t=subname;
    return SelectSub(s, t);
}

bool iniparser::SelectSub(string& nodename, string& subname)
{
	if(listob.empty())
        return false;
    list<iniline>::iterator it=listob.begin();
    for(; it->IsNode && it->data.node.name==nodename; ++it)
    {
        if(it==listob.end())
        {
            return false;
        }
    } // skip until it is found
    for(; !it->IsNode && it!=listob.end(); ++it)
    {
        if(it->data.sub.name==subname)
        {
            listIter=it;
            return true;
        }
    }
    return false;
}

bool iniparser::SetNodeName(char* nodename, char* newname)
{
    string s=nodename;
    string t=newname;
    return SetNodeName(s, t);
}

bool iniparser::SetNodeName(string& nodename, string& newname)
{
	if(listob.empty())
        return false;
    SelectNode(nodename);
    iniline *l=&(*listIter);
    strcpy(l->data.node.name, newname.c_str());
    return true;
}

bool iniparser::SetSubName(char* nodename, char* subname, char* newname)
{
    string s=nodename;
    string t=newname;
    string u=subname;
    return SetSubName(s, u, t);
}

bool iniparser::SetSubName(string& nodename, string& subname, string& newname)
{
	if(listob.empty())
        return false;
    SelectSub(nodename, subname);
    iniline *l=&(*listIter);
    strcpy(l->data.sub.name, newname.c_str());
    return true;
}

bool iniparser::DeleteNode(char* nodename)
{
    string s=nodename;
    return DeleteNode(s);
}

bool iniparser::DeleteNode(string& nodename)
{
	if(listob.empty())
        return false;
    if(!DoesNodeExist(nodename))
        return false;
    SelectNode(nodename);
    for(list<iniline>::iterator it=listIter; it!=listob.end();)
    {
        listob.erase(it++);
        if(it->IsNode)
            break;
    }
}

bool iniparser::DeleteSub(char* nodename, char* subname)
{
    string s=nodename;
    string u=subname;
    return DeleteSub(s, u);
}

bool iniparser::DeleteSub(string& nodename, string& subname)
{
	if(!DoesSubExist(nodename, subname))
		return false;
	SelectSub(nodename, subname);
	listob.erase(listIter);
	listIter=listob.begin();
	return true;
}
bool iniparser::SetSubValue(string& nodename, string& subname, string& value)
{
	if(!DoesSubExist(nodename, subname))
		return false;
	SelectSub(nodename, subname);
	iniline *l=getdata();
	strcpy(l->data.sub.value, value.c_str());
	return true;
}


bool iniparser::AddNode(string& nodename)
{
	iniline l;
	l.IsNode=true;
	strcpy(l.data.node.name, nodename.c_str());
	listob.push_back(l);
	return true;
}
bool iniparser::AddSub(string& nodename, string& subname, string& value)
{
	if(!DoesNodeExist(nodename))
        return false;
    SelectNode(nodename);
    iniline l;
    l.IsNode=false;
    strcpy(l.data.sub.name, subname.c_str());
    strcpy(l.data.sub.value, value.c_str());
    list<iniline>::iterator it=listIter;
    listob.insert(++it, l);
    return true;
}

string iniparser::Compile()
{
    string outStr("");
    for(list<iniline>::iterator it=listob.begin(); it!=listob.end(); ++it)
    {
        if(it->IsNode)
        {
            // if it is a node, we just copy node name
            outStr+=it->data.node.name;
            outStr+="\r\n";
        }else if(!it->IsNode)
        {
            // if it is a sub, we have to copy like this:
            // <name> = <value> \n
            outStr+=it->data.sub.name;
            outStr+=" = ";
            outStr+=it->data.sub.value;
            outStr+="\r\n";
        }
    }
    return outStr;
}
