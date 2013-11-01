#include <iostream>
#include "..\file\file.h"
#include "filelist.h"

using namespace std;

string testFile("<FILELIST>\nC:\\test\\testfile.h\nD:\\testfile.2d\n");

int main()
{
    fileList l;
    l.load(testFile);
    file f, g;
    f.setpath("C:\\asdfasdf");
    g.setpath("D:\\asdfasdf");
    l.addfile(f);
    l.updatefile(f, g);
    cout << l.compile() << endl;
    return 0;
}
