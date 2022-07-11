#include <iostream>
#include <windows.h>
#include <fstream>
using namespace std;

int main(int argc,char** argv)
{
    char* fn="test.txt";
    char* fp="copy.txt";
    char* fm="move.txt";
    //make file
    ofstream os(fn);
    if(!os.is_open())
    {
        printf("%s open error\n",fn);
        return 0;
    }
    os<<"file data";
    os.close();

    //copy
    if(!CopyFile(fn,fp,false)){
        printf("copy file error : %d\n",GetLastError());
    }
    if(!DeleteFile(fn)){
        printf("delete file error : %d\n",GetLastError());
    }
    if(!MoveFile(fp,fm)){
        printf("move file error : %d\n",GetLastError());
    }


    cout<<"File type of ["<<fm<<"] : "<<GetFileAttributesA(fm)<<endl;

    //set file to hidden(2)
    SetFileAttributesA(fm,2);


    return 0;
}
