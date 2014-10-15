
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

// a array lock to lock all kinds of container 
// with a bit memory comsumption

#ifndef LOCK_ARRAY_H
#define LOCK_ARRAY_H

#include <stdint.h>
#include <pthread.h>
#include "bit_array.h"

#include "domob_log.h"

namespace SELIB_UTIL {

//
//A really, *really* small spinlock for fine-grained locking of lots
//of teeny-tiny data. one bit for a block data
//
struct ArrayLock 
{
public:

  bool create(uint32_t len)
  {
	return _ba.create(len);
  }

  bool try_op(uint32_t pos, bool set = true) 
  {
	volatile uint8_t * lock = NULL;
	uint32_t offset = 0;
	bool ret = _ba.test_bit(pos, &offset, &lock);
	
	asm volatile("mfence" : : : "memory");
	if (ret == set) {
		return false;
	} 

	uint8_t old = *lock;
	volatile uint8_t expected = old;

	asm volatile("mfence" : : : "memory");
	if (set) {
		// aoid interuption by other thread which will update the lock
		// set the bit to zero
		clear_bit(old, offset);
		set_bit(expected, offset);
	} else {
		clear_bit(expected, offset);
	}

	asm volatile("mfence" : : : "memory");

	return __sync_bool_compare_and_swap(lock, old, expected);

  }

  void lock(uint32_t pos) 
  {
    do {
      while (_ba.test_bit(pos)) {
		asm volatile("mfence" : : : "memory");
		pthread_yield();
      }
    } while (!try_op(pos, true));
  }

  bool unlock(uint32_t pos) 
  {
	  do {

		if (!_ba.test_bit(pos)) {
			DOMOB_WRITE_FATAL_LOG("lock err[lock is unlocked by other thread]");
			return false;
		}

	  } while (!try_op(pos, false));

	  return true;
  }

private:
 
  inline void set_bit(volatile uint8_t & lock, uint32_t & offset)
  {
	  uint8_t mask = 1;
	  mask = (uint8_t)(mask << (7u - offset));

	  lock |= mask;
  }

  inline void clear_bit(volatile uint8_t & lock, uint32_t & offset)
  {

	  uint8_t mask = 1;
	  mask = (uint8_t)(mask << (7u - offset));
	  mask = (uint8_t)~mask;

	  lock &= mask;
  }


  BitArray  _ba;

};

}

#endif
