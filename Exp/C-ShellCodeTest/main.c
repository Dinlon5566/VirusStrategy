/*
Shellcode can be changed to work with any windows distribution by changing the address of Beep in kernel32.dll
Addresses for SP1 and SP2

-
*/

#include <stdio.h>

unsigned char beepsp1[] =
"\x55\x89\xE5\x83\xEC\x18\xC7\x45\xFC"
"\x10\xC9\xEA\x77"                      //Address \x10\xC9\xEA\x77 = SP1
"\xC7\x44\x24\x04"
"\xE8\x03"                              //Length \xE8\x03 = 1000 (1 second)
"\x00\x00\xC7\x04\x24"
"\xE8\x03"                              //Frequency  \xE8\x03 = 1000
"\x00\x00\x8B\x45\xFC\xFF\xD0\xC9\xC3";

unsigned char beepsp2[] =
"\x55\x89\xE5\x83\xEC\x18\xC7\x45\xFC"
"\x53\x8A\x83\x7C"                      //Address \x53\x8A\x83\x7C = SP2
"\xC7\x44\x24\x04"
"\xD0\x03"                              //Length \xD0\x03 = 2000 (2 seconds)
"\x00\x00\xC7\x04\x24"
"\x01\x0E"                              //Frequency \x01\x0E = 3585
"\x00\x00\x8B\x45\xFC\xFF\xD0\xC9\xC3";
char x86_nullfree_msgbox[] =
"\x31\xd2\xb2\x30\x64\x8b\x12\x8b\x52\x0c\x8b\x52\x1c\x8b\x42"
"\x08\x8b\x72\x20\x8b\x12\x80\x7e\x0c\x33\x75\xf2\x89\xc7\x03"
"\x78\x3c\x8b\x57\x78\x01\xc2\x8b\x7a\x20\x01\xc7\x31\xed\x8b"
"\x34\xaf\x01\xc6\x45\x81\x3e\x46\x61\x74\x61\x75\xf2\x81\x7e"
"\x08\x45\x78\x69\x74\x75\xe9\x8b\x7a\x24\x01\xc7\x66\x8b\x2c"
"\x6f\x8b\x7a\x1c\x01\xc7\x8b\x7c\xaf\xfc\x01\xc7\x68\x79\x74"
"\x65\x01\x68\x6b\x65\x6e\x42\x68\x20\x42\x72\x6f\x89\xe1\xfe"
"\x49\x0b\x31\xc0\x51\x50\xff\xd7";
int main()
{
    void (*function)();
    *(long*)&function = (long)x86_nullfree_msgbox;
    function();
}

