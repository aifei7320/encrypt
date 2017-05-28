#include "encrypt.h"

DvEncrypt::DvEncrypt(const string &key, const string &iv,
		const string &src_file, const string &dst_file) {

	key_ = key;
	iv_ = iv;
	src_file_ = src_file;
	dst_file_ = dst_file;

	encrypt();
}
int DvEncrypt::encrypt() {
	int length = filesize(src_file_);
	unsigned char * buffer = new unsigned char[length];


	readfile(buffer, src_file_, length);

	AES_KEY aes_enc;
	if (AES_set_encrypt_key((unsigned char*) key_.c_str(), 128, &aes_enc) < 0) {
		//LOG(ERROR)<<"KEY ISN'T CORRECT"<<endl;
	}

	unsigned char *enc_out = new unsigned char[length];
	int num = 0;
	AES_cfb128_encrypt((unsigned char *) buffer, enc_out,
			(unsigned long) length, &aes_enc, (unsigned char *) iv_.c_str(),
			&num, AES_ENCRYPT);

	writefile(enc_out, dst_file_, length);

	delete[] buffer;
	delete[] enc_out;
	return 1;
}
DvEncrypt::~DvEncrypt() {

}

DvDecrypt::DvDecrypt(const string &key, const string &iv,
		const string &src_file, const string &dst_file) {
	key_ = key;
	iv_ = iv;
	src_file_ = src_file;
	dst_file_ = dst_file;
	decrypt();
}
int DvDecrypt::decrypt() {
	int length = filesize(src_file_);
	unsigned char * buffer = new unsigned char[length];
	readfile(buffer, src_file_, length);
	AES_KEY aes_enc;
	if (AES_set_encrypt_key((unsigned char*) key_.c_str(), 128, &aes_enc) < 0) {
        std::cout<<"KEY ISN'T CORRECT"<<endl;
	}
	int num=0;
	unsigned char *dec_out = new unsigned char[length];
	AES_cfb128_encrypt(buffer, dec_out, (unsigned long) length, &aes_enc, (unsigned char *) iv_.c_str(),
			&num, AES_DECRYPT);
    writefile(dec_out, dst_file_, length);
	int cnt=0;
    cnt += fromstring(&model_info_.magic_num, &dec_out[cnt],1);
	cnt += fromstring(&model_info_.version,&dec_out[cnt],1);
	cnt += fromstring(&model_info_.reserve,&dec_out[cnt],1);
	cnt += fromstring(&model_info_.patchsize,&dec_out[cnt],1);
	cnt += fromstring(&model_info_.model_num,&dec_out[cnt],1);
    
    printf("magic_num:%d\n", model_info_.magic_num);
    printf("version:%f\n", model_info_.version);
    printf("reserve:%d\n", model_info_.reserve);
    printf("patchsize:%d\n", model_info_.patchsize);
    printf("model_num:%d\n", model_info_.model_num);
	for(int i=0;i<model_info_.model_num;i++){

		int magic_num;
		cnt += fromstring(&magic_num,&dec_out[cnt],1);
        if (magic_num != model_info_.magic_num ){
            delete[] buffer;
            delete[] dec_out;
            return -1;
        }

		int model_size;
		cnt += fromstring(&model_size,&dec_out[cnt],1);
		abstractFile(&dec_out[cnt],model_size);
		cnt += model_size;

	}
	delete[] buffer;
	delete[] dec_out;
	return 1;
}
const ModelInfo & DvDecrypt::getModelInfo(){
	return model_info_;
}
int DvDecrypt::abstractFile(unsigned char *src,int size){

	unsigned char * data = new unsigned char[size];

	memcpy((char *)data,(char *)src,size);
	char *file = new char[100];
    time_t rawtime;
    time(&rawtime);

	sprintf(file,"/tmp/%s",ctime(&rawtime));
	FILE *wfile = fopen(file,"wb");
    if(wfile == NULL){
        printf("open write file failed\n");
    }

	fwrite(data,sizeof(unsigned char),size,wfile);

	fclose(wfile);

	delete[] file;
	delete[] data;
	return 1;
}
DvDecrypt::~DvDecrypt() {

}
