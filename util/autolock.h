
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

#ifndef SELIB_AUTOLOCK
#define SELIB_AUTOLOCK

#include <pthread.h>

namespace SELIB_UTIL {

// to define a variable with one type
// in one field will hold one mutex
// example1:
// {
//      autolock<int> a;
//      ...                 // hang here
//      autolock<int> b;
// }
//
// example2:
// {
//      autolock<int> a;
// }
// ...                     // ok
//      qutolock<int> b;
//
// example3:
// {
//      autolock<int> a;
//      ...                 // ok
//      autolock<char> b;
// }
//
template<class TYPE>
class autolock 
{
public:
    autolock()
    {
        pthread_mutex_lock(&_mutex);
    }
    ~autolock()
    {
        pthread_mutex_unlock(&_mutex);
    }
private:
    static pthread_mutex_t _mutex;
};

template<class TYPE>
pthread_mutex_t autolock<TYPE>::_mutex = PTHREAD_MUTEX_INITIALIZER;

}

#endif


