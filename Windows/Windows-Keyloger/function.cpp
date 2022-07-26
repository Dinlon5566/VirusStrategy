#include "function.h"

string intToString(int num)
{
    char buf[4];
    itoa(num,buf,10);
    return string(buf);
}
string getCurrDir()
{
    char *curdir=new char[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,curdir);
    string res(curdir);
    delete[] curdir;
    return res;
}
string getSelfPath()
{
    char *selfdir=new char[MAX_PATH];
    GetModuleFileName(NULL,selfdir,MAX_PATH);
    string res(selfdir);
    delete[] selfdir;
    return res;
}

string dirBasename(string path)
{
    if(path.empty())
        return "";

    if(path.find('\\')==string::npos)
        return path;

    //remove last '\'
    if(path.substr(path.length()-1)=="\\")
        path=path.substr(0,path.length()-1);

    size_t pos=path.find_last_of("\\");

    if(pos != string::npos)
        path = path.substr(0, pos);

    if(path.substr(path.length() - 1) == "\\")
        path = path.substr(0, path.length() - 1);

    return path;
}
