#include<fstream>
#include<string>
#include<Windows.h>
#include<shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
using namespace std;
int main() {
	char buf[0xFF];
	HMODULE hMod = GetModuleHandleA(NULL);
	GetModuleFileNameA(hMod, buf, 0xFF);
	PathStripPathA(buf);
	fstream f;
	f.open("selfDel.bat", ios::out);
	string data = string(":startExe\r\nif not exist ") + buf + " goto done\r\ndel /f /q " + buf + "\r\ngoto startExe\r\n:done\r\ndel /f /q %0";
	f.write(data.data(), data.size());
	f.close();
	ShellExecuteA(NULL, "open", "selfDel.bat", NULL, NULL, SW_HIDE);
}
