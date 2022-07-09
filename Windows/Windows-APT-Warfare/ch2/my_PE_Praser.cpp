/**
* my_PE_parser
* by Dinlon5566
* Reference aaaddress1
*/
#include <iostream>
#include <windows.h>
bool readBinFile(char *fileName,char* &binFileTarget,DWORD &fileSize)
{
    FILE* fp;
    // Open the file and ptr to fp.
    if(fp= fopen(fileName,"rb"))
    {
        // Set the pointer to the end of the file, and then request the pointer position.
        fseek(fp,0,SEEK_END);
        fileSize=ftell(fp);

        // Read data into the binFileTarget
        binFileTarget= new char[fileSize+1];
        fseek(fp,0,SEEK_SET);
        fread(binFileTarget,sizeof(char),fileSize,fp);

        return 1;
    }
    return 0;
}

void pePraser(char* peBinFilePtr)
{

    // Structure defined in <winnt.h>.
    IMAGE_DOS_HEADER* dosHeader= (IMAGE_DOS_HEADER*) peBinFilePtr;
    IMAGE_NT_HEADERS* ntHeader= (IMAGE_NT_HEADERS*)((size_t) dosHeader+dosHeader->e_lfanew);

    // Check magic code
    if(dosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
    {
        puts("DOS Header error!");
        return;
    }
    if(ntHeader->Signature!=IMAGE_NT_SIGNATURE)
    {
        puts("NT Header error!");
        return;
    }

    IMAGE_OPTIONAL_HEADER* optionalHeader =  &ntHeader->OptionalHeader;
    puts("---Optional Header detail---");
    printf("Optional Header Type : %s\n",typeid(optionalHeader).name());
    if(optionalHeader)
    {
        printf("ImageBase : %p\n",optionalHeader->ImageBase);
        printf("SizeOfImage : %x\n",optionalHeader->SizeOfImage);
        printf("Entry Point : %p\n", optionalHeader->ImageBase + optionalHeader -> AddressOfEntryPoint);
    }

    puts("---Section Info---");
    IMAGE_SECTION_HEADER* sectionHeader= (IMAGE_SECTION_HEADER*)((size_t)ntHeader+sizeof(*ntHeader));
    printf("#ID - %8s - %8s- %8s\n","name","PtrToData","SizeOfData");

    for(size_t i=0; i<ntHeader->FileHeader.NumberOfSections; i++)
    {
        printf("#%.2x - %8s - %.8x - %.8x \n",i,sectionHeader[i].Name,sectionHeader[i].PointerToRawData,sectionHeader[i].SizeOfRawData);
    }

}


int main(int argc,char** argv)
{
    char *binaryData;
    DWORD binaryDataSize;

    if(argc!=2)
    {
        puts("You need input a file");
    }
    else if(readBinFile(argv[1],binaryData,binaryDataSize))
    {
        printf("File Name : %s\n",argv[1]);
        printf("File Size : %d bytes\n",binaryDataSize);
        pePraser(binaryData);
    }
    else
    {
        puts("read file failure");
    }
    return 0;
}
