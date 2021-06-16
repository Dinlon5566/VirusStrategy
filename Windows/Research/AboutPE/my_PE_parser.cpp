/**
* my_PE_parser
* by Dinlon5566
* Reference aaaddress1
*/
#include <iostream>
#include<windows.h>
#pragma warning( disable:4996 )
using namespace std;

bool readBinaryFile(const char fileName[],char*& binaryFile,DWORD& binaryFileSize){
    if(FILE* fp=fopen(fileName,"rb")){
        fseek(fp,0,SEEK_END);
        binaryFileSize=ftell(fp);
        fseek(fp,0,SEEK_SET);
        binaryFile=new char[binaryFileSize+1];
        fread(binaryFile,sizeof(char),binaryFileSize,fp);
        return 1;
    }else return 0;
}

void peParser(char* binaryFile){
    IMAGE_DOS_HEADER* dosHad=(IMAGE_DOS_HEADER*) binaryFile;
    IMAGE_NT_HEADERS* ntHad=(IMAGE_NT_HEADERS*)((size_t)dosHad+dosHad-> e_lfanew);
    if(dosHad->e_magic==IMAGE_DOS_SIGNATURE && ntHad->Signature==IMAGE_NT_SIGNATURE){

        IMAGE_SECTION_HEADER* secHad=(IMAGE_SECTION_HEADER*)((size_t)ntHad+sizeof(*ntHad));
        for(size_t i = 0; i < ntHad ->FileHeader.NumberOfSections; i++){
            printf("[+]\t#%.2x - %8s - %.8x -%.8x\n",i,secHad[i].Name,secHad[i].PointerToRawData,secHad[i].SizeOfRawData);
        }

    }else{
        puts("[!]not a PE file");
    }
}

int main(int argc,char** argv)
{
    char* binaryData;
    DWORD dataSize;
    if(argc!=2){
        puts("[!]input file");

#if 0
        puts("[tmp]use \"hello_world.exe\"\?[Y/N]");
        char tmp;
        cin>>tmp;
        if(tmp=='Y'){
            argv[1]="hello_world.exe";
            goto AccessPoint;
        }
#endif // tmp

    }else{
        AccessPoint:
        if(readBinaryFile(argv[1],binaryData,dataSize)){
            printf("[+] file name @ %s\n",argv[1]);
            printf("[+] file size @ %p\n",dataSize);
            peParser(binaryData);
        }else{
            puts("[!]readBinaryData Error");
        }

    }
    puts("Process finish : )");
    return 0;
}
