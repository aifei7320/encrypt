
#ifndef ENCRYPT_H_
#define ENCRYPT_H_

//#include <glog/logging.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <openssl/aes.h>
#include <list>
#include <map>
#include <time.h>

using namespace std;
static void readfile(unsigned char *buffer, const string &model_file,
		long length) {

	FILE *rFile = fopen(model_file.c_str(), "rb");
	fread(buffer, sizeof(unsigned char), length, rFile);
	fclose(rFile);
}
static void writefile(unsigned char *buffer, const string &model_file,
		long length) {

	FILE *rFile = fopen(model_file.c_str(), "wb");
	fwrite(buffer, sizeof(unsigned char), length, rFile);
	fclose(rFile);
}
static long filesize(const string &model_file) {
	ifstream is;
	is.open(model_file.c_str(), ios::binary);

	// get length of file:
	is.seekg(0, std::ios::end);

	long length = is.tellg();
	is.seekg(0, std::ios::beg);
	is.close();
	return length;
}

typedef enum {
    Txt=0,
    Modle,
    Pt,
    Info,
}model_type;

typedef enum{
    High=0,
    Middle,
    Low

}classification_level;

template<typename T>
static int fromstring(T *dst,unsigned char * src,int length){
	int size = sizeof(T)*length;

	 char *temp = new  char[size];

	memcpy(temp,src,size);
	*dst = *(T*)temp;
	delete[] temp;
	return size;
}

typedef struct{
    int magic_num;
	float version;
	int model_num;
	int reserve;
	int patchsize;
	map<int,int>/*modeltype classification_num*/class_num_for_modeltype;
}ModelInfo;

typedef struct{
    int magic_num;
    long size;
    int reserve;
}header_info;

class DvEncrypt {
public:
	DvEncrypt(const string &key, const string &iv, const string &src_file,
			const string &dst_file);
	~DvEncrypt();
private:
	string key_;
	string iv_;
	string src_file_;
	string dst_file_;
	int encrypt();
};
class DvDecrypt {
public:
	DvDecrypt(const string &key, const string &iv, const string &src_file,
			const string &dst_file);
    const list<string> getDecryptFile();
	~DvDecrypt();
	const ModelInfo& getModelInfo();

private:
	string key_;
	string iv_;
	string src_file_;
	string dst_file_;
	ModelInfo model_info_;
    list<string> decryptFileName;//用当前时间生成四个文件名称。传给调用者
	int decrypt();
	int abstractFile(unsigned char *data,int size);

};
#endif /* ENCRYPT_H_ */
