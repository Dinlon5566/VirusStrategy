#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
    DWORD disks=GetLogicalDrives();
    char dname[4]="A:";
    for(int i=0; i<26; i++)
    {
        if(disks&(1<<i))
        {
            dname[0]=i+'A';
            printf("%s\ttype %d\n",dname,GetDriveTypeA(dname));
        }
    }
    return 0;
}
