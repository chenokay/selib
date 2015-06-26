
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

#ifndef RESOURCE_STACK
#define RESOURCE_STACK

#include <new>
#include <lifo.h>
#include <lifo.hpp>
#include "log_print.h"

namespace SELIB_UTIL {

// resource holder for high frequency resource usage
// in multi-thread program 
template<typename RESOURCE>
struct resource_holder_t : public Lifo::Node {
RESOURCE *  _p;
};

// producer in default
struct producer_null_t {
    char * operator() ()
    {
        return NULL;
    }
    void destroy(void *) {}
};

// ResourceStack has following characters:
// 1. thread safe stack
// 2. redefine producer action
// 3. lock-free
template<typename RESOURCE, typename PRODUCER = producer_null_t>
class ResourceStack {
public:
    typedef resource_holder_t<RESOURCE> hoder_t;
    ResourceStack():_producer(NULL) {}
    ~ResourceStack()
    {
        destroy();
    }

    bool push(RESOURCE * in)
    {
        if (NULL == in) {
            return false;
        }
        hoder_t * hoder = (hoder_t*)_empty.pop();
        if (NULL == hoder) {
            return false;
        }
        hoder->_p = in;
        _aviable.push(hoder);
        return true;
    }
    
    RESOURCE * pop()
    {
        hoder_t * hoder = (hoder_t*)_aviable.pop();
        if (NULL == hoder) {
            return NULL;
        }
        RESOURCE * back = hoder->_p;
        hoder->_p = NULL;
        _empty.push(hoder);

        return back;
    }

    // create bucket for using 
    // as a queue
    bool create(uint32_t bucket_size) {
        for (uint32_t i = 0;i < bucket_size; ++ i) {
            hoder_t * hoder = new (std::nothrow)hoder_t;
            if (NULL == hoder) {
                return false;
            }
            _empty.push(hoder);
        }
        return true;
    }
    
    // three following functions are related 
    // resource producing inner
    RESOURCE * force_pop()
    {
        RESOURCE * b = pop();
        if (NULL == b) {
            b = produce();
        }
        return b;
    }

    // bind instance to procuder as 
    // needing data commucation
    void bind(PRODUCER * produce)
    {
        _producer = produce;
    }

    RESOURCE * produce()
    {
        hoder_t * hoder = NULL;
        RESOURCE * back = NULL;
        if (NULL == _producer) {
            back = _producer_inst();
        } else {
            back = (*_producer)();
        }
        if (NULL == back) {
            return NULL;
        }
        hoder = new (std::nothrow)hoder_t;
        if (NULL == hoder) {
            return NULL;
        }

        hoder->_p = NULL;

        _empty.push(hoder);

        return back;
    }
        
private:
    void destroy()
    {
        while(true) {
            hoder_t * hoder = (hoder_t*)_empty.pop();
            if (NULL == hoder) {
                break;
            } else {
                delete hoder;
            }
        }

        while(true) {
            hoder_t * hoder = (hoder_t*)_aviable.pop();
            if (NULL == hoder) {
                break;
            } else {
                if (NULL != _producer) {
                    _producer_inst.destroy(hoder->_p);
                } else {
                    (*_producer).destroy(hoder->_p);
                }
                delete hoder;
            }
        }
    }
    // if there not exist bind instance pointed by "_producer" 
    // obviously use the default instance _producer_inst 
    PRODUCER * _producer;
    PRODUCER  _producer_inst;
    Lifo _aviable;
    Lifo _empty;
};

}

#endif
