/*************************************************************************
    > File Name: main.cpp
    > Author: zxf
    > Mail: zhengxiaofeng333@163.com 
    > Created Time: Fri 26 May 2017 10:46:05 PM PDT
 ************************************************************************/

#include "encrypt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
    FILE *f;
    unsigned char *writeinfo=NULL;
    unsigned char *tmp=NULL;
    int writeSize=0;
    unsigned char temp[20];
    int migic_num;

    classification_level cl;

    ModelInfo m, n;
    m.magic_num = 8012;
    m.version = 1.0;
    m.model_num = 4;
    m.reserve =1;
    m.patchsize =1;

    
    f = fopen("ModelInfo", "wb"); 
    memcpy(&temp[0], &m.magic_num, sizeof(int)); 
    memcpy(&temp[4], &m.version, sizeof(float)); 
    memcpy(&temp[8], &m.model_num, sizeof(int)); 
    memcpy(&temp[12], &m.reserve, sizeof(int)); 
    memcpy(&temp[16], &m.patchsize, sizeof(int)); 
    fwrite(temp, 20, 1, f);
    fclose(f);
    f=fopen("test.de", "rb");

    fread(temp, 20, 1, f);
    memcpy(&n.magic_num, &temp[0], 4);
    memcpy(&n.version, &temp[4], 4);
    memcpy(&n.model_num, &temp[8], 4);
    memcpy(&n.reserve, &temp[12], 4);
    memcpy(&n.patchsize, &temp[16], 4);
    printf("%d, %f, %d, %d, %d\n", n.magic_num, n.version, n.model_num, n.reserve, n.patchsize);
    fclose(f);

    string ckey = "helloworldkeyzxf";
    string ivec = "goodbyworldkeyxf";

    string ensrname = "test.cpp";
    string endesname = "test";

    string desrname = "test";
    string dedesname = "test.de";

    DvEncrypt de(ckey, ivec, ensrname, endesname);
    DvDecrypt dd(ckey, ivec, endesname, dedesname);
    return 0;
}

