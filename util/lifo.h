/*
 *   Written 2008, 2009, 2010 by <mgix@mgix.com>
 *   This code is in the public domain
 *   See http://www.mgix.com/snippets/?LockFree for details
 *
 *   A lock-free LIFO stack for x86 CPUs
 *   Requires a CPU that supports cmpxchg(8|16)
 */
#ifndef __LIFO_H__
#define __LIFO_H__

#include <assert.h>
#include <inttypes.h>

namespace SELIB_UTIL {
    
struct Lifo {
    struct Node { Node *volatile next; };
    Node *volatile head;
    uintptr_t volatile sync;
    Lifo();
    void     push(Node *node);
    uint32_t tryPush(Node *node);
    Node     *tryPop(uint32_t &success);
    Node     *pop();
} __attribute__((aligned(2*sizeof(uintptr_t)), packed));

}   
#endif // __LIFO_H__
