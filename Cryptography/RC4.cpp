#include <iostream>
#include <windows.h>
using namespace std;

void show_PUCHAR(PUCHAR data,int length=0)
{
    if(length==0)
    {
        length=strlen((char*)data);
    }
    for(size_t i=0; i<length; i+=16)
    {
        printf("%08X |",i);
        for(size_t j=i; j<i+16; j++)
        {
            if(j>=length)
            {
                printf(" XX");
            }
            else
            {
                printf(" %02X",data[j]);
            }
        }
        printf(" | ");
        for(size_t j=i; j<i+16; j++)
        {
            if(j>=length)
            {
                printf(".");
                continue;
            }
            UCHAR c=data[j];
            if(c<32|c>127)
                c='.';
            printf("%c",c);
        }
        puts("");
    }
}

PUCHAR makeSbox(PUCHAR key,unsigned int keyLength=0){
    if(keyLength==0){
        keyLength=strlen((char*)key);
    }
    //make new sbox
    PUCHAR sbox=new UCHAR[256];
    for(int i=0;i<256;i++){
        sbox[i]=i;
    }

    // swap sbox with key

    for(int i=0,j=0;i<256;i++){
        j=(j+sbox[i]+key[(i%keyLength)])%256;
        swap(sbox[i],sbox[j]);
    }
    return sbox;
}

PUCHAR rc4_sboxEncrypt(PUCHAR data,PUCHAR sbox,int length){
    PUCHAR res=new UCHAR[256];
    for(int i=-1,j=0,p=0,k;p<length;p++){
        i=(i+1)%256;
        j=(j+sbox[i])%256;
        swap(sbox[i],sbox[j]);
        k=(sbox[i]+sbox[j])%256;
        res[p]=data[p]^sbox[k];
    }
    return res;
}
int main()
{
    char text[256]="hello world";
    char password[256]="password";

    puts("");

    PUCHAR sbox=makeSbox((PUCHAR)password);
    show_PUCHAR(sbox,256);

    printf("Encode\n");
    PUCHAR encrypt=rc4_sboxEncrypt((PUCHAR)text,sbox,strlen(text));
    show_PUCHAR(encrypt);

    printf("Decode\n");
    sbox=makeSbox((PUCHAR)password);
    PUCHAR decrypt=rc4_sboxEncrypt((PUCHAR)encrypt,sbox,strlen(text));
    show_PUCHAR(decrypt);

    return 0;
}
