#ifndef CRYPTO_MD5_H
#define CRYPTO_MD5_H

extern "C" {
#include "solar-md5.h"
}

#include <vector>
#include <cstdint>
#include <string>

class MD5 {
public:

	MD5(int flags = 0);

	static uint64_t hash(const std::string &text);
	static std::vector<uint8_t> calculate(std::vector<uint8_t> &data);
	void add(std::vector<uint8_t> &data, int offset = 0);

	template <typename T> void add(T b) {
		MD5_Update(&ctx, &b, sizeof(T));	
	}

	std::vector<uint8_t> get();

private:
	MD5_CTX ctx;
	int flags;
};


#endif // CRYPTO_MD5_H
