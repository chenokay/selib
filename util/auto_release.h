
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

#ifndef AUTO_RELEASE_H
#define AUTO_RELEASE_H

#include <stdint.h>
#include <new>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

namespace SELIB_UTIL {
// RAII to release all kinds of resource and handle automatically. 

class AutoClose 
{
	public:
		AutoClose(FILE * p) : _p(p) {}
		~AutoClose() {
			if (NULL != _p) {
				fclose(_p);
			}
		}
	private:
		FILE * _p;
};

class AutoDesClose 
{
	public:
		AutoDesClose(int fd) : _fd(fd) {}
		~AutoDesClose() {
			if (0 != _fd && -1 != _fd) {
				close(_fd);
			}
		}
	private:
		int _fd;
};

class AutoUnmap 
{
	public:
		AutoUnmap(void * vmem, size_t size) : _vmem(vmem), _size(size) {}
		~AutoUnmap() {
			if (0 != _size && NULL != _vmem) {
				munmap(_vmem, _size);
			}
		}
	private:
		void * _vmem;
		size_t _size;
};

template<typename TYPE>
class AutoBufDeleter 
{
	public:
		AutoBufDeleter(TYPE * p) : _p(p) {}
		~AutoBufDeleter() {
			delete [] _p;
		}
	private:
		TYPE * _p;
};

}

#endif
