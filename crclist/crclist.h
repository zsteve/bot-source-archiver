#ifndef CRCLIST_H
#define CRCLIST_H

#include "crcablefile.h"
#include <list>
#include "..\exceptions\exception.h"

#define STANDARD_CRCLIST_HEADER "<CRCLIST>"

class crcList
{
    public:
    class entry{
        // this is a CRC entry.
        private:
            unsigned int crc;
            string path;
        public:
            entry& operator=(entry& rhs)
            {
                this->path=rhs.getpath();
                this->crc=rhs.getcrc();
            }
            entry()
            {
                crc=NULL;
                path="";
            }
            entry(const entry& src)
            {
                path=src.path;
                crc=src.crc;
            }
            ~entry()
            {

            }
            string getpath()
            {
                return path;
            }
            void setpath(string path)
            {
                this->path=path;
            }
            unsigned int getcrc()
            {
                return crc;
            }
            void setcrc(unsigned int crc)
            {
                this->crc=crc;
            }
    };
    private:
    list<entry> crclst;
    string getline(string& src, int lineindex);
    entry* parse(string& line);
    public:
    crcList()
    {
        crclst.clear();
    }
    bool load(string& data) throw (FormatInvalidException);
	bool load(char* data) throw (FormatInvalidException);
    bool addfile(file& newFile);
	unsigned long getcrc(crcAbleFile& chkFile);
	bool checkfile(crcAbleFile& chkFile);
    bool updatefile(crcAbleFile& updFile) throw (FileNonExistentException);
    bool deletefile(crcAbleFile& delFile) throw (FileNonExistentException);
    string compile() throw (exception);
    list<entry>& getlist()
    {
        return crclst;
    }
};

#endif
