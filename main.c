#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

//string to int
//printf("%08x\n",foo("0xff"))
unsigned int foo(const char * s) {
 unsigned int result = 0;
 int c ;
 if ('0' == *s && 'x' == *(s+1)) { s+=2;
  while (*s) {
   result = result << 4;
   if (c=(*s-'0'),(c>=0 && c <=9)) result|=c;
   else if (c=(*s-'A'),(c>=0 && c <=5)) result|=(c+10);
   else if (c=(*s-'a'),(c>=0 && c <=5)) result|=(c+10);
   else break;
   ++s;
  }
 }
 return result;
}

//���o2������
int getPower(int value){
    int count=0;
    int chk=1;
    while(chk){
        value=value/2;
        if (value<2){
            chk=0;
        }
        count++;
    }
    return count;
}


//�Q�i����G�i��
char * toBin(int value){
    char *s = malloc(50 * sizeof(char));
    itoa(value, s, 2);
    return s;
}

//�G�i����Q���i��
void binaryToHex(const char *inStr, char *outStr) {
    // outStr must be at least strlen(inStr)/4 + 1 bytes.
    static char hex[] = "0123456789ABCDEF";
    int len = strlen(inStr) / 4;
    int i = strlen(inStr) % 4;
    char current = 0;
    if(i) { // handle not multiple of 4
        while(i--) {
            current = (current << 1) + (*inStr - '0');
            inStr++;
        }
        *outStr = hex[current];
        ++outStr;
    }
    while(len--) {
        current = 0;
        for(i = 0; i < 4; ++i) {
            current = (current << 1) + (*inStr - '0');
            inStr++;
        }
        *outStr = hex[current];
        ++outStr;
    }
    *outStr = 0; // null byte
}

//�����M�g
int fun1(int ramSize_MB,int cacheMemSize_KB,int cacheBlockSize_B,int memAddress){
    //to byte
    ramSize_MB=ramSize_MB*pow(2, 20);
    cacheMemSize_KB=cacheMemSize_KB*pow(2, 10);

    //
    printf("\n");
    printf("������(��):%d\n",ramSize_MB/cacheMemSize_KB);

    //�u��
    printf("�C�Ӱ϶��j�p(KB):%d\n",cacheMemSize_KB/1024);

    //�C�Ӱ϶��t��(��)�u��
    printf("�C�Ӱ϶��t��(��)�u��:%d\n",cacheMemSize_KB/cacheBlockSize_B);
    //int totalPath=cacheMemSize_KB/cacheBlockSize_B;

    //�O�����m����
    printf("�`����(bits):%d\n",getPower(ramSize_MB));
    int memLength=getPower(ramSize_MB);

    //
    printf("�u���ݭn?bits�w�}:%d\n",getPower(cacheMemSize_KB/cacheBlockSize_B));
    int pathNeedBits=getPower(cacheMemSize_KB/cacheBlockSize_B);

    //
    printf("�r�ջݭn?bits�w�}:%d\n",getPower(cacheBlockSize_B));
    int wordsNeedBits=getPower(cacheBlockSize_B);

    //���Ҥj�p(�ѤU�S�Ψ쪺�w�}�Ŷ�)
    printf("���ҥi�Τj�p(bits):%d\n",memLength-pathNeedBits-wordsNeedBits);
    int tagSize=memLength-pathNeedBits-wordsNeedBits;

    //�B�z��J���O����
    printf("\nMEM(HEX):0x%x\n",memAddress);
    printf("MEM(BIN):%s\n",toBin(memAddress));
    char *memAddress_bin=malloc(50 * sizeof(char));
    memAddress_bin=toBin(memAddress);

    //��}�C����>���o�첾
    int length=0;
    while(memAddress_bin[length]!=0){length++;} // array final char \0
    length++;
    length--;length--;

    char words[50]="";
    char cachePathNum[50]="";
    char tag[50]="";


    int flag;
    for(flag=wordsNeedBits-1;flag>-1;flag--,length--){
        words[flag]=memAddress_bin[length];
    }

    for(flag=pathNeedBits-1;flag>-1;flag--,length--){
        cachePathNum[flag]=memAddress_bin[length];
    }

    for(;length>-1;length--){
        tag[length]=memAddress_bin[length];
    }



    //���G

    binaryToHex(tag,tag);
    binaryToHex(words,words);
    binaryToHex(cachePathNum,cachePathNum);
    printf("\n����:0x%s �֨����u:0x%s �r��:0x%s",tag,cachePathNum,words);

    return 0;
}

//���X���p�M�g
int fun2(int ramSize_MB,int cacheMemSize_KB,int cacheBlockSize_B,int memAddress){
    //to byte
    int ramSize_byte=ramSize_MB*pow(2, 20);
    int cacheMemSize_byte=cacheMemSize_KB*pow(2, 10);

    //�O�����m����
    printf("�`����(bits):%d\n",getPower(ramSize_byte));
    int memLength=getPower(ramSize_byte);

    printf("�r�ջݭn?bits�w�}:%d\n",getPower(cacheBlockSize_B));
    int wordsNeedBits=getPower(cacheBlockSize_B);

   int kPath;
   printf("�V��(k):");
   scanf("%d",&kPath);


    //�C�Ӱ϶��t��(��)�u��
    int blockPath;
    printf("�C�Ӱ϶��t��(��)�u��:%d\n",blockPath=cacheMemSize_byte/cacheBlockSize_B);

    int memBlockvalue=ramSize_byte/kPath;
   printf("�C�Ӷ��X(v)�t��(byte):%d\n",memBlockvalue);

    int tagNeedBits;
    printf("����(�줸):%d\n",tagNeedBits=getPower(memBlockvalue/blockPath));
    printf("\n��}�榡:\n����:%d�줸 ���X:%d �r��:%d�줸\n",tagNeedBits,memLength-wordsNeedBits-tagNeedBits,wordsNeedBits);

    //�B�z��J���O����
    printf("\nMEM(HEX):0x%x\n",memAddress);
    printf("MEM(BIN):%s\n",toBin(memAddress));
    char *memAddress_bin=malloc(50 * sizeof(char));
    memAddress_bin=toBin(memAddress);

    //��}�C����>���ѿ�X
    int length=0;
    while(memAddress_bin[length]!=0){length++;} // array final char \0
    length++;

    length--;length--;//need 0~23 to count a array

    int i,j,k;
    char tag[50]="";
    char set[50]="";
    char words[50]="";

    int tmp=wordsNeedBits-1;
    for(i=length;i>length-wordsNeedBits;i--){
        words[tmp]=memAddress_bin[i];
       // printf("tmp:%d\n",tmp);
        tmp--;
    }
    //printf("W:%s\n",words);
    length=i;

    tmp=memLength-wordsNeedBits-tagNeedBits-1;//���X����
     for(i=length;i>length-(memLength-wordsNeedBits-tagNeedBits);i--){
        set[tmp]=memAddress_bin[i];
        //printf("tmp:%d\n",tmp);
        tmp--;
     }
     //printf("set:%s",set);
     length=i;

     for(i=length;i>0-1;i--){
        tag[i]=memAddress_bin[i];
     }


    //printf("����:%s ���X:%s �r��%s\n",tag,set,words);

     binaryToHex(words,words);
     binaryToHex(set,set);
     binaryToHex(tag,tag);
    printf("����:0x%s ���X:0x%s �r��0x%s\n",tag,set,words);


    return 0;
}

//�������p�M�g
int fun3(int ramSize_MB,int cacheMemSize_KB,int cacheBlockSize_B,int memAddress){

    //to byte
    int ramSize_byte=ramSize_MB*pow(2, 20);
    int cacheMemSize_byte=cacheMemSize_KB*pow(2, 10);

    //�O�����m����
    printf("�`����(bits):%d\n",getPower(ramSize_byte));
    int memLength=getPower(ramSize_byte);

    printf("�r�ջݭn?bits�w�}:%d\n",getPower(cacheBlockSize_B));
    int wordsNeedBits=getPower(cacheBlockSize_B);

    int memBlockVal=pow(2,memLength-wordsNeedBits);
    printf("�D�O����϶��`��:%d\n",memBlockVal);

    int tagNeedBits=memLength-wordsNeedBits;
    printf("����:%d�줸,�r��:%d�줸\n",tagNeedBits,wordsNeedBits);

    //�B�z��J���O����
    printf("\nMEM(HEX):0x%x\n",memAddress);
    printf("MEM(BIN):%s\n",toBin(memAddress));
    char *memAddress_bin=malloc(50 * sizeof(char));
    memAddress_bin=toBin(memAddress);

    //
    int i,j,tmp;
    char words_bin[50]="";
    char tag_bin[50]="";

     //��}�C����>���o�첾
    int length=0;
    while(memAddress_bin[length]!=0){length++;} // array final char \0
    length++;

    length--;length--; //�վ�q�}�C0�}�l



    for(i=length,tmp=wordsNeedBits-1;
            i>length-wordsNeedBits;
            i--,tmp--){
       words_bin[tmp]=memAddress_bin[i];
    }

    length=length-wordsNeedBits;
    for(i=length;i>0-1;i--){
        tag_bin[i]=memAddress_bin[i];
    }

    printf("����%s ",tag_bin);
    printf("�r��%s\n",words_bin);

    binaryToHex(tag_bin,tag_bin);
    binaryToHex(words_bin,words_bin);

    printf("����0x%s ",tag_bin);
    printf("�r��0x%s\n",words_bin);

    //getch();

    return 0;
}


int main(int argc, char** argv) {
    //

	int ramSize_MB;
	int cacheMemSize_KB;
	int cacheBlockSize_B;
	int funSelect;

	//
    int memAddress;
    char memAddress_s[50];

	//�Ѽƿ�J;
	int loop=1;
    while(loop){
    printf("RAM size(MB)�G");
    scanf("%d", &ramSize_MB);

    printf("Cache mem size(KB)�G");
    scanf("%d", &cacheMemSize_KB);

    printf("Cache block size(Byte)�G");
    scanf("%d", &cacheBlockSize_B);

    printf("Input a mem address(HEX)�G\nExample:\n0x16339C\n0xFF7FFC 0x017FFC\n0xFFFFFC 0x16339C\n");
    scanf("%s", memAddress_s);
    memAddress=foo(memAddress_s);

    printf("select function\n1.�����M�g 2.���X���p�M�g 3.�������p�M�g\n");
    scanf("%d", &funSelect);


    /*
    ramSize_MB=16;
    cacheMemSize_KB=64;
    cacheBlockSize_B=4;
    memAddress=foo("0xff7ffc");//string to int
    funSelect=2;
    */


    //
     if(funSelect==1){
        fun1(ramSize_MB,cacheMemSize_KB,cacheBlockSize_B,memAddress);
     }
     else if(funSelect==2){
          fun2(ramSize_MB,cacheMemSize_KB,cacheBlockSize_B,memAddress);
     }
     else if(funSelect==3){
        fun3(ramSize_MB,cacheMemSize_KB,cacheBlockSize_B,memAddress);
     }
     else{
        return 0;
     }
     printf("\n\ncontinue? Y=1,N=0\n");
     scanf("%d",&loop);
     system("@cls||clear");//clean screen
    }


	return 0;
}
