#include <iostream>
#include "ftplib.hpp"

using namespace std;

int main()
{
    ftpconn* f;
    cout << "FtpWrapper Unit Test" << endl;
    try{
        f=new ftpconn("127.0.0.1", "steve", "123");
    }catch(ConnectionFail e)
    {
        cout << "ConnectionFail" << endl;
    }catch(LogonFail e)
    {
        cout << "LogonFail" << endl;
    }
    cout << "Logon Success" << endl;
    cout << f->sendfile("/test/test.txt" ,"C:\\ReadmeDOS.txt");
    return 0;
}
