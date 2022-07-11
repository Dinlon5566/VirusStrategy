#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
    char astr[]="hello world";
    char *pstr=nullptr;
    DWORD strl=strlen(astr);
    printf("astr : [%s]\n",astr);
    pstr=(char*)HeapAlloc(GetProcessHeap(),0,strl+1);
    if(pstr==nullptr){
        puts("heapalloc error");
        return 1;
    }
    strcpy_s(pstr,strl+1,astr);
    printf("pstr : [%s]\n",pstr);
    HeapFree(GetProcessHeap(),0,pstr);
    return 0;
}
