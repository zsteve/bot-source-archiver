#ifndef SETTING_H
#define SETTING_H
#include <string>
#include "..\..\serializable\serializable.h"
using namespace std;

class setting : serializable
{
public:
	bool isLoaded;
	struct{
		string addr, user, pass, path;
	}ftp;
	struct{
		string filename;
		int version;
		bool autoincrement;
	}file;
	struct{
		bool incremental;
	}option;
	setting()
	{
		isLoaded=false;
		ftp.addr=ftp.user=ftp.pass=ftp.path="";
	}
	string getFilename(){
		string outStr="";
		char* tempNum=new char[64];
		sprintf(tempNum, "%i", file.version);
		outStr+=file.filename+"_"+tempNum;
		delete[] tempNum;
		if(option.incremental)
		{
			outStr+="_incr";
		}else{
			outStr+="_full";
		}
		outStr+=".zip";
		return outStr;
	}
	int incrementVersion(){
		file.version++;
		{
			char* tempNum=new char[64];
			sprintf(tempNum, "%i", file.version);
			string tempNumStr=tempNum;
			myParser.setvalue("[file]", "version", tempNumStr);
			delete[] tempNum;
		}
		return file.version;
	}
	string serialize(){
		myParser.setvalue("[ftp]", "addr", ftp.addr);
		myParser.setvalue("[ftp]", "user", ftp.user);
		myParser.setvalue("[ftp]", "pass", ftp.pass);
		myParser.setvalue("[ftp]", "path", ftp.path);
		myParser.setvalue("[file]", "filename", file.filename);
		{
			char* tempNum=new char[64];
			sprintf(tempNum, "%i", file.version);
			string tempNumStr=tempNum;
			myParser.setvalue("[file]", "version", tempNumStr);
			delete[] tempNum;
		}
		{
			char* tempNum=new char[64];
			sprintf(tempNum, "%i", (int)file.autoincrement);
			string tempBoolStr=tempNum;
			myParser.setvalue("[file]", "autoincrement", tempBoolStr);
			delete[] tempNum;
		}
		{
			char* tempNum=new char[64];
			sprintf(tempNum, "%i", (int)option.incremental);
			string tempBoolStr=tempNum;
			myParser.setvalue("[option]", "incremental", tempBoolStr);
			delete[] tempNum;
		}
		string outStr("");
		for(list<line>::iterator it=myParser.getdatalist().begin(); it!=myParser.getdatalist().end(); ++it)
		{
			if(it->isNode)
			{
				// item is a node
				outStr+=it->data.node.name;
				outStr+="\r\n";
				continue;
			}
			if(!it->isNode)
			{
				outStr+=it->data.sub.name;
				outStr+=" = ";
				outStr+=it->data.sub.value;
				outStr+="\r\n";
			}
		}
		return outStr;
	}
	bool deserialize(char* data)
	{
		string *addr, *user, *pass, *path;
		string *filename, *version, *autoinc;
		string *incr;
		string strData=data;
		if(!myParser.load(strData))
		{
			return false;
		}
		addr=myParser.getvalue("[ftp]", "addr");
		user=myParser.getvalue("[ftp]", "user");
		pass=myParser.getvalue("[ftp]", "pass");
		path=myParser.getvalue("[ftp]", "path");
		filename=myParser.getvalue("[file]", "filename");
		version=myParser.getvalue("[file]", "version");
		autoinc=myParser.getvalue("[file]", "autoincrement");
		incr=myParser.getvalue("[option]", "incremental");
		if(addr)
			ftp.addr=*addr;
		else
			ftp.addr="";

		if(user)
			ftp.user=*user;
		else
			ftp.user="";

		if(pass)
			ftp.pass=*pass;
		else
			ftp.pass="";

		if(path)
			ftp.path=*path;
		else
			ftp.path="";

		if(filename)
			file.filename=*filename;
		else
			file.filename="";

		if(version)
			file.version=atoi(version->c_str());
		else
			file.version=1;
		if(autoinc)
			file.autoincrement=(bool)atoi(autoinc->c_str());
		else
			file.autoincrement=false;

		if(incr)
			option.incremental=(bool)atoi(incr->c_str());
		else
			option.incremental=false;
		isLoaded=true;
		return true;
	}
};

#endif