
#ifndef INIPARSE_H
#define INIPARSE_H
#define MAX_INIPATH 300
#include <list>
#include <string>
using namespace std;

typedef struct{
	bool IsNode;
	union {
		struct{
			char name[80];
		}node;
		struct{
			char name[80];
			char value[MAX_INIPATH];
		}sub;
	}data;
}iniline;

class iniparser{
	// private
private:
	enum linetype{LINE_NODE, LINE_SUB};
	list<iniline> listob;
	list<iniline>::iterator listIter;
	iniline* getdata();
	iniline Parse(string& line);
	string GetLine(string& src, int lineindex);
public:
	// public
	iniparser()
	{
	    listIter=listob.begin();
	}
	bool Load(string& src);
	bool SelectNode(string& nodename);
	bool SelectNode(char* nodename);
	bool DoesNodeExist(string& nodename);
	bool DoesNodeExist(char* nodename);
	bool GetSub(string& nodename, string& subname);
	bool GetSub(char* nodename, char* subname);
	bool DoesSubExist(string& nodename, string& subname);
	bool DoesSubExist(char*nodename, char* subname);
	bool SelectSub(string& nodename, string& subname);
	bool SelectSub(char* nodename, char* subname);
	bool SetNodeName(string& nodename, string& newname);
	bool SetNodeName(char* nodename, char* newname);
	bool SetSubName(string& nodename, string& subname, string& newname);
	bool SetSubName(char* nodename, char* subname, char* newname);
	bool DeleteNode(string& nodename);
	bool DeleteNode(char* nodename);
	bool DeleteSub(string& nodename, string& subname);
	bool DeleteSub(char* nodename, char* subname);
	bool SetSubValue(string& nodename, string& subname, string& value);
	bool AddNode(string& nodename);
	bool AddSub(string& nodename, string& subname, string& value);
	string Compile();
	/* inline functions */
	char *GetSubValue()
	{
		return listIter->data.sub.value;
	}
};
#endif
