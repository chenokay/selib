
//selib is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//	author chenokay@gmail.com
//

#ifndef SELIB_BIT_ARRAY_H
#define SELIB_BIT_ARRAY_H

#include <stdint.h>
#include <string.h>
#include <new>
#include <pthread.h>

namespace SELIB_UTIL {

class BitArray {
	public:
		typedef uint8_t TYPE;

		BitArray() : _bits_buf(NULL) {}

		bool create(uint32_t len)
		{
			_buf_len = len;

			uint64_t num = (len / (sizeof(TYPE) * 8u)) + 1u;
			_bits_buf = new (std::nothrow) TYPE[num];

			if (NULL == _bits_buf) {
				return false;
			}

			bzero((void *)_bits_buf, sizeof(TYPE) * num);
			_pow_of_two = 0;

			uint32_t bits_len = sizeof(TYPE) * 8;
			// a % b, if b is pow of 2, it's eqeual with a & (b - 1)
			_offset_mask = bits_len - 1;
			// begion with 0
			_align_len = bits_len - 1;

			bits_len = bits_len >> 1;
			for (;bits_len > 0; ) {
				++ _pow_of_two;
				bits_len = bits_len >> 1;
			}

			return true;
		}

		~BitArray() 
		{
			delete [] _bits_buf;
		}

		bool test_bit(uint32_t pos, uint32_t * offset = NULL, volatile TYPE * volatile * ap = NULL)
		{
			if (pos >= _buf_len) {
				return false;
			}

			uint32_t os = 0;

			volatile TYPE * tp = get_raw_bit(pos, &os);

			if (NULL != tp && NULL != ap) {
				*ap = tp;
			}
			if (NULL != offset) {
				*offset = os;
			}

			TYPE t = *tp;

			t = (TYPE)(t >> (_align_len - os));
			t = t & 0x1;

			return 1 == t ? true : false;
		}

		bool set_bit(uint32_t pos)
		{
			if (pos >= _buf_len) {
				return false;
			}

			uint32_t idx = 0;
			uint32_t offset = 0;

			get_position(pos, &idx, &offset);

			TYPE mask = 1;
			mask = (TYPE)(mask << (_align_len - offset));

			_bits_buf[idx] |= mask;

			return true;
		}

		bool clear_bit(uint32_t pos)
		{
			if (pos >= _buf_len) {
				return false;
			}

			uint32_t idx = 0;
			uint32_t offset = 0;

			get_position(pos, &idx, &offset);

			TYPE mask = 1;
			mask = (TYPE)(mask << (_align_len - offset));
			mask = (TYPE)(~mask);

			_bits_buf[idx] &= mask;

			return true;
		}

		// pos begion with 0, which indicate the first position
		volatile TYPE * get_raw_bit(uint32_t pos, uint32_t * offset = NULL)
		{

			if (pos >= _buf_len) {
				return NULL;
			}

			uint32_t idx = 0;
			uint32_t os;

			get_position(pos, &idx, &os);

			if(NULL != offset) {
				*offset = os;
			}

			return (volatile TYPE *)&(_bits_buf[idx]);
		}

		void dump()
		{
			uint64_t num = (_buf_len / (sizeof(TYPE) * 8u)) + 1u;
			for (uint32_t i = 0; i < num; ++ i) {
				printf("thread_[%ld] dump pos %d is:%u\n", 
						pthread_self(), i, _bits_buf[i]);
			}
		}

private:

		void get_position(uint32_t & pos, 
				uint32_t * align_idx, uint32_t * offset)
		{
			*align_idx = pos >> _pow_of_two;
			*offset = pos & _offset_mask;
		}

		volatile TYPE * _bits_buf;
		uint32_t _pow_of_two;
		uint32_t _offset_mask;
		uint32_t _align_len;

		uint32_t _buf_len;
};

}

#endif
