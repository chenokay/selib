// this file is introduced from folly of FB
// use these lightweight and fast hash function 
// to improve the performance and simplify programming procedure 
// as it can cater the need

#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

#include <stdint.h>
#include <string>

namespace SELIB_UTIL {
	/*
	 * Fowler / Noll / Vo (FNV) Hash
	 *     http://www.isthe.com/chongo/tech/comp/fnv/
	 */

	const uint32_t FNV_32_HASH_START = 2166136261UL;
	const uint64_t FNV_64_HASH_START = 14695981039346656037ULL;

	inline uint32_t fnv32(const char* s,
			uint32_t hash = FNV_32_HASH_START) {
		for (; *s; ++s) {
			hash += (hash << 1) + (hash << 4) + (hash << 7) +
				(hash << 8) + (hash << 24);
			hash ^= *s;
		}
		return hash;
	}

	inline uint32_t fnv32_buf(const void* buf,
			int n,
			uint32_t hash = FNV_32_HASH_START) {
		const char* char_buf = reinterpret_cast<const char*>(buf);

		for (int i = 0; i < n; ++i) {
			hash += (hash << 1) + (hash << 4) + (hash << 7) +
				(hash << 8) + (hash << 24);
			hash ^= char_buf[i];
		}

		return hash;
	}

	inline uint64_t fnv64(const char* s,
			uint64_t hash = FNV_64_HASH_START) {
		for (; *s; ++s) {
			hash += (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) +
				(hash << 8) + (hash << 40);
			hash ^= *s;
		}
		return hash;
	}

	inline uint64_t fnv64_buf(const void* buf,
			int n,
			uint64_t hash = FNV_64_HASH_START) {
		const char* char_buf = reinterpret_cast<const char*>(buf);

		for (int i = 0; i < n; ++i) {
			hash += (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) +
				(hash << 8) + (hash << 40);
			hash ^= char_buf[i];
		}
		return hash;
	}

}

#endif
