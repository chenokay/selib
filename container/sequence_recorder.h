
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

#ifndef SELIB_SEQUENCE_RECORDER
#define SELIB_SEQUENCE_RECORDER

#include <stdint.h>

namespace SELIB_UTIL {

// manage a sequence of number in a circle buffer
template <class TYPE, int LEN = 10> class SequenceRecorder {
public:
    SequenceRecorder() 
	{
		_beg = 0;
		_cur = 0;
		_len = 0;
		_started = false;
	}
	~SequenceRecorder() {}

   void append(TYPE in)
   {
	   if (_started) {
		_cur = (_cur + 1) % LEN;
		_buf[_cur] = in;
		if (_beg == _cur) {
			_beg = (_beg + 1) % LEN;
		} 
	   } else {
		   _buf[_cur] = in;
		   _started = true;
	   }
		++ _len;
   }

   void clear()
   {
	   _beg = _cur;
	   _len = 0;
	   _started = false;
   }
    
   int get_every(TYPE * out, int len) const 
   {
		for (int i = 0, j = _beg; i < len && i < get_len();++i, ++j) 
		{
			out[i] = _buf[(j % LEN)];
		}

		return get_len();
   }

   TYPE get_sum() const 
   {
	   TYPE sum = 0;
	   for (int i = 0, j = _beg; i < get_len(); ++i, ++j) 
	   {
		   sum += _buf[(j % LEN)];
	   }

	   return sum;
   }

   int get_len() const 
   {
	   return _len >= LEN ? LEN : _len;
   }

   TYPE get_avg() const 
   {
	   if (0 == get_len()) {
		   return 0;
	   }
	   return (get_sum() / get_len());
   }

   int get_count(TYPE e) const 
   {
	   int count = 0;
	   for (int i = 0, j = _beg; i < get_len(); ++i, ++j) 
	   {
		   if (e == _buf[(j % LEN)]) 
		   {
			   ++ count;
		   }
	   }

	   return count;
   }

   double get_density(TYPE e) const 
   {
	   if (0 == get_len()) {
		   return 0;
	   }

		return ((double)get_count(e)/get_len());
   }

private:
    TYPE _buf[LEN];

	int _beg;
	int _cur;
	int _len;

	bool _started;
};

} // end namespace

#endif
