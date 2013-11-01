#include <iostream>
#include "iniparse.h"

using namespace std;
string s;
char test[]={"[asdf] \nasdf = 100 \n"};
int main()
{
    cout << "Hello world!" << endl;
    iniparser i;
    s=test;
    i.Load(s);
   // i.SelectNode("[asdf]");
   // i.SelectSub("[asdf]", "asdf");
   // cout << i.GetSubValue();
    cout << i.Compile();
    return 0;
}
