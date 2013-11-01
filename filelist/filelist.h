#ifndef FILELIST_H
#define FILELIST_H

#include "..\crclist\crcablefile.h"
#include <list>
#include "..\exceptions\exception.h"

#define STANDARD_FILELIST_HEADER "<FILELIST>"

class fileList
{
	public:
	    class entry{
        private:
        string path;
        public:
        entry()
        {
            path="";
        }
        ~entry(){}
        entry(const entry& src)
        {
            path=src.path;
        }
        entry& operator=(const entry& rhs)
        {
            path=rhs.path;
            return *this;
        }
        string getpath(){return path;}
        void setpath(string path){this->path=path;}
	  };
    private:
        string getline(string& src, int lineindex);
        list<entry> filelst;
    public:
        bool load(string& data) throw (FormatInvalidException);
		bool load(char* data) throw (FormatInvalidException);
        bool addfile(file& newFile);
		bool checkfile(file& chkFile);
        bool updatefile(file& oldFile, file& updFile);
        bool removefile(file& delFile);
		list<entry>& getList(){return filelst;}
        string compile();
    protected:
};

#endif
