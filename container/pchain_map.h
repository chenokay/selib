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
////	author chenokay@gmail.com

// a thread-safe hash map which use one bit to lock one bucket
//
#ifndef SELIB_PCHAIN_MAP_H
#define SELIB_PCHAIN_MAP_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "chain_hash.h"
#include "array_lock.h"

namespace SELIB_UTIL {

template <typename Type> 
class PChainMap 
{
public:
	PChainMap() {}
	~PChainMap() {}

	int create(uint32_t bucket_num)
	{
		_bucket_num = bucket_num;
		_array_lock.create(_bucket_num);
		return _dict.create(_bucket_num);
	}

	bool insert(unsigned long key_sign, const Type *p_data)
	{
		uint32_t bucket_pos = key_sign % _bucket_num;

		AutoLock aulock(bucket_pos, &_array_lock);

		return _dict.insert(key_sign, *p_data);
	}
	
	Type * find(unsigned long key_sign, Type * res = NULL)
	{
		if (NULL == res) {
			SELIB_WRITE_FATAL_LOG("fail to read[need res to store return value]");
			return NULL;
		}

		return _find(key_sign, res);

	}

	const Type * find(unsigned long key_sign, Type * res = NULL) const
	{
		if (NULL == res) {
			SELIB_WRITE_FATAL_LOG("fail to read[need res to store return]");
			return NULL;
		}
		return _find(key_sign, res);

	}

private:

	Type * _find(unsigned long key_sign, Type * res)
	{
		uint32_t bucket_pos = key_sign % _bucket_num;
		AutoLock aulock(bucket_pos, &_array_lock);

		Type * rt = _dict.find(key_sign);

		if (NULL == rt) {
			return NULL;
		}
		*res = *rt;

		return res;
	}

	class AutoLock {
		public:
			AutoLock(uint32_t pos,
					SELIB_UTIL::ArrayLock * al) : _al(al), _pos(pos) 
		{
			if (NULL != _al) {
				_al->lock(_pos);
			} 
		}
			~AutoLock() 
			{
				if (NULL != _al) {
					_al->unlock(_pos);
				}
			}
		private:
			SELIB_UTIL::ArrayLock * _al;
			uint32_t _pos;
	};

	ChainHashMap<Type> _dict;
	SELIB_UTIL::ArrayLock _array_lock;
	uint32_t _bucket_num;
};

}; 
using SELIB_UTIL::PChainMap;

#endif
