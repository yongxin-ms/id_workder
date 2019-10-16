#include <stdio.h>
#include <string>
#include "base64.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("usage: %s 'string'\n", argv[0]);
		return 1;
	}

	const std::string msg(argv[1]);

	base64::Base64Encrypt base64enc(msg.data(), msg.size());
	const std::string encode = base64enc.GetString();
	printf("encode:[%s]\n", encode.data());

	base64::Base64Decrypt base64dec(encode.data(), encode.size());
	std::string decode((const char*)base64dec.PlainText(), base64dec.GetSize());
	printf("decode:[%s]\n", decode.data());

	return 0;
}
