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

//取得2的次方
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


//十進制轉二進制
char * toBin(int value){
    char *s = malloc(50 * sizeof(char));
    itoa(value, s, 2);
    return s;
}

//二進制轉十六進制
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

//直接映射
int fun1(int ramSize_MB,int cacheMemSize_KB,int cacheBlockSize_B,int memAddress){
    //to byte
    ramSize_MB=ramSize_MB*pow(2, 20);
    cacheMemSize_KB=cacheMemSize_KB*pow(2, 10);

    //
    printf("\n");
    printf("切分成(塊):%d\n",ramSize_MB/cacheMemSize_KB);

    //線路
    printf("每個區塊大小(KB):%d\n",cacheMemSize_KB/1024);

    //每個區塊含有(條)線路
    printf("每個區塊含有(條)線路:%d\n",cacheMemSize_KB/cacheBlockSize_B);
    //int totalPath=cacheMemSize_KB/cacheBlockSize_B;

    //記憶體位置長度
    printf("總長度(bits):%d\n",getPower(ramSize_MB));
    int memLength=getPower(ramSize_MB);

    //
    printf("線路需要?bits定址:%d\n",getPower(cacheMemSize_KB/cacheBlockSize_B));
    int pathNeedBits=getPower(cacheMemSize_KB/cacheBlockSize_B);

    //
    printf("字組需要?bits定址:%d\n",getPower(cacheBlockSize_B));
    int wordsNeedBits=getPower(cacheBlockSize_B);

    //標籤大小(剩下沒用到的定址空間)
    printf("標籤可用大小(bits):%d\n",memLength-pathNeedBits-wordsNeedBits);
    int tagSize=memLength-pathNeedBits-wordsNeedBits;

    //處理輸入的記憶體
    printf("\nMEM(HEX):0x%x\n",memAddress);
    printf("MEM(BIN):%s\n",toBin(memAddress));
    char *memAddress_bin=malloc(50 * sizeof(char));
    memAddress_bin=toBin(memAddress);

    //算陣列長度>取得位移
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



    //結果

    binaryToHex(tag,tag);
    binaryToHex(words,words);
    binaryToHex(cachePathNum,cachePathNum);
    printf("\n標籤:0x%s 快取路線:0x%s 字組:0x%s",tag,cachePathNum,words);

    return 0;
}

//集合關聯映射
int fun2(int ramSize_MB,int cacheMemSize_KB,int cacheBlockSize_B,int memAddress){
    //to byte
    int ramSize_byte=ramSize_MB*pow(2, 20);
    int cacheMemSize_byte=cacheMemSize_KB*pow(2, 10);

    //記憶體位置長度
    printf("總長度(bits):%d\n",getPower(ramSize_byte));
    int memLength=getPower(ramSize_byte);

    printf("字組需要?bits定址:%d\n",getPower(cacheBlockSize_B));
    int wordsNeedBits=getPower(cacheBlockSize_B);

   int kPath;
   printf("向數(k):");
   scanf("%d",&kPath);


    //每個區塊含有(條)線路
    int blockPath;
    printf("每個區塊含有(條)線路:%d\n",blockPath=cacheMemSize_byte/cacheBlockSize_B);

    int memBlockvalue=ramSize_byte/kPath;
   printf("每個集合(v)含有(byte):%d\n",memBlockvalue);

    int tagNeedBits;
    printf("標籤(位元):%d\n",tagNeedBits=getPower(memBlockvalue/blockPath));
    printf("\n位址格式:\n標籤:%d位元 集合:%d 字組:%d位元\n",tagNeedBits,memLength-wordsNeedBits-tagNeedBits,wordsNeedBits);

    //處理輸入的記憶體
    printf("\nMEM(HEX):0x%x\n",memAddress);
    printf("MEM(BIN):%s\n",toBin(memAddress));
    char *memAddress_bin=malloc(50 * sizeof(char));
    memAddress_bin=toBin(memAddress);

    //算陣列長度>屁股輸出
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

    tmp=memLength-wordsNeedBits-tagNeedBits-1;//集合長度
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


    //printf("標籤:%s 集合:%s 字組%s\n",tag,set,words);

     binaryToHex(words,words);
     binaryToHex(set,set);
     binaryToHex(tag,tag);
    printf("標籤:0x%s 集合:0x%s 字組0x%s\n",tag,set,words);


    return 0;
}

//完全關聯映射
int fun3(int ramSize_MB,int cacheMemSize_KB,int cacheBlockSize_B,int memAddress){

    //to byte
    int ramSize_byte=ramSize_MB*pow(2, 20);
    int cacheMemSize_byte=cacheMemSize_KB*pow(2, 10);

    //記憶體位置長度
    printf("總長度(bits):%d\n",getPower(ramSize_byte));
    int memLength=getPower(ramSize_byte);

    printf("字組需要?bits定址:%d\n",getPower(cacheBlockSize_B));
    int wordsNeedBits=getPower(cacheBlockSize_B);

    int memBlockVal=pow(2,memLength-wordsNeedBits);
    printf("主記憶體區塊總數:%d\n",memBlockVal);

    int tagNeedBits=memLength-wordsNeedBits;
    printf("標籤:%d位元,字組:%d位元\n",tagNeedBits,wordsNeedBits);

    //處理輸入的記憶體
    printf("\nMEM(HEX):0x%x\n",memAddress);
    printf("MEM(BIN):%s\n",toBin(memAddress));
    char *memAddress_bin=malloc(50 * sizeof(char));
    memAddress_bin=toBin(memAddress);

    //
    int i,j,tmp;
    char words_bin[50]="";
    char tag_bin[50]="";

     //算陣列長度>取得位移
    int length=0;
    while(memAddress_bin[length]!=0){length++;} // array final char \0
    length++;

    length--;length--; //調整從陣列0開始



    for(i=length,tmp=wordsNeedBits-1;
            i>length-wordsNeedBits;
            i--,tmp--){
       words_bin[tmp]=memAddress_bin[i];
    }

    length=length-wordsNeedBits;
    for(i=length;i>0-1;i--){
        tag_bin[i]=memAddress_bin[i];
    }

    printf("標籤%s ",tag_bin);
    printf("字組%s\n",words_bin);

    binaryToHex(tag_bin,tag_bin);
    binaryToHex(words_bin,words_bin);

    printf("標籤0x%s ",tag_bin);
    printf("字組0x%s\n",words_bin);

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

	//參數輸入;
	int loop=1;
    while(loop){
    printf("RAM size(MB)：");
    scanf("%d", &ramSize_MB);

    printf("Cache mem size(KB)：");
    scanf("%d", &cacheMemSize_KB);

    printf("Cache block size(Byte)：");
    scanf("%d", &cacheBlockSize_B);

    printf("Input a mem address(HEX)：\nExample:\n0x16339C\n0xFF7FFC 0x017FFC\n0xFFFFFC 0x16339C\n");
    scanf("%s", memAddress_s);
    memAddress=foo(memAddress_s);

    printf("select function\n1.直接映射 2.集合關聯映射 3.完全關聯映射\n");
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
