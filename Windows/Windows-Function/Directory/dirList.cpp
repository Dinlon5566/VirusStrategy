#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <tchar.h>
using namespace std;

int main(int argc,char* argv[])
{

    WIN32_FIND_DATA findFileData;
    HANDLE handle;

    strcat(argv[1],"\\*");
    if(argc!=2){
        printf("Usage: %s [target File]",argv[0]);
        return 0;
    }

    printf("Target File : %s\n",argv[1]);
    handle=FindFirstFile(argv[1],&findFileData);

    if(handle==INVALID_HANDLE_VALUE){
        printf("Find file faile : %d\n",GetLastError());
        return 0;
    }
    printf("\t    \t\\%s\n",findFileData.cFileName);

    while(FindNextFile(handle,&findFileData)!=0){
        if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
            printf("\t<DIR>\t\\%s\n",findFileData.cFileName);
        }else{
            printf("\t    \t\\%s\n",findFileData.cFileName);
        }
    }

    FindClose(handle);

    return 0;
}
