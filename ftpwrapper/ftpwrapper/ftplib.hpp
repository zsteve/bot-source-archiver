#include "ftplib.h"
#include <exception>

#ifndef FTPLIB_HPP
#define FTPLIB_HPP

using namespace std;

class ConnectionFail : public exception
{

};

class LogonFail : public exception
{

};


class ftpconn
{
    private:
    netbuf* connection;
    string host, user, pass;
    public:
    ftpconn() throw (ConnectionFail)
    {
        host="";
    }
    ftpconn(string host, string user, string pass) throw (ConnectionFail)
    {
        FtpInit();
        this->host=host;
        this->user=user;
        this->pass=pass;
        if(FtpConnect(host.c_str(), &connection)==false)
        {
            // connection failed
            throw ConnectionFail();
        }
        if(FtpLogin(user.c_str(), pass.c_str(), connection)==false)
        {
            // logon failed
            throw LogonFail();
        }
        FtpClose(connection);
    }
    bool testconnection()
    {
        if(FtpConnect(host.c_str(), &connection)==false)
        {
            // connection failed
           return false;
        }
        FtpClose(connection);
    }
    bool sendfile(string path, string src, int mode=FTPLIB_ASCII)
    {
        FtpInit();
        if(FtpConnect(host.c_str(), &connection)==false)
        {
            // connection failed
           return false;
        }
        if(FtpLogin(user.c_str(), pass.c_str(), connection)==false)
        {
            // logon failed
            return false;
        }
        if(FtpPut(src.c_str(), path.c_str(), mode, connection)==false)
        {
            FtpClose(connection);
            return false;
        }
        FtpClose(connection);
        return true;
    }
    bool getfile(string& path, string& targ, int mode=FTPLIB_ASCII)
    {
        if(FtpConnect(host.c_str(), &connection)==false)
        {
            // connection failed
           return false;
        }
        if(FtpLogin(user.c_str(), pass.c_str(), connection)==false)
        {
            // logon failed
            return false;
        }
        if(FtpGet(targ.c_str(), path.c_str(), mode, connection)==false)
        {
            FtpClose(connection);
            return false;
        }
        FtpQuit(connection);
        return true;
    }
    protected:
};

#endif
