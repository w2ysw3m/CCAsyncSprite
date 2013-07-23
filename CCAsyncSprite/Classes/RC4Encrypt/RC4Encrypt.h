#ifndef _ENCRYPT_RC4_
#define _ENCRYPT_RC4_
#include <string.h>


#define BOX_LEN 256
#define ENCRYPT_PWD "rockifone_powerful_ileader"

class RC4Encrypt{

public:
	static int GetKey(const unsigned char* pass, int pass_len, unsigned char *out);
	static int RC4(const unsigned char* data, int data_len, const unsigned char* key, int key_len, unsigned char* out, int* out_len);
	static void swap_byte(unsigned char* a, unsigned char* b);
	static char* Encrypt(const char* szSource);
	static char* Decrypt(const char* szSource);
	static char* ByteToHex(const unsigned char* vByte, const int vLen);
	static unsigned char* HexToByte(const char* szHex);
};
#endif // #ifndef _ENCRYPT_RC4_
