#ifndef SELIB_PQUEUE_H
#define SELIB_PQUEUE_H

#include <pthread.h>
#include "lock_free_atomic_ops.h"
#include "autolock.h"

namespace SELIB_UTIL {

template<class Type>
struct Node {
	Type val;
	Node<Type> * next; 
	Node() 
	{
		next = NULL;
	}
};

// PQueue is high performance queue
// which has only a few instructions in critical 
// area. 
// Queue is built with simple link.
template<class Type>
class PQueue 
{
public:
	typedef Node<Type> QueueNode; 

	PQueue() 
	{
		this->size = 0;
		this->tail = &(this->head);
		flushed = false;
	}
	~PQueue() 
	{
		// delete dummy node
		delete head.next;
	}

	int reserve(uint32_t len)
	{
		this->length = len;
		return 0;
	}

	int flush()
	{
		// add dummy node
		Type t;
		this->push(t);
		flushed = true;
		return 0;
	}

	bool all_poped()
	{
		// only dummy node is left
		return flushed && (size == 1);
	}

	int try_push(Type x)
	{
		do {
			if (this->size > this->length) {
				pthread_yield();
			}
		} while (this->size > this->length);

		return this->push(x);
	}

	struct PQeue_write_t {};
	int push(Type x)
	{
		QueueNode * qn = new (std::nothrow) QueueNode();
		if (NULL == qn) {
			return -1;
		}
		qn->val = x;
		qn->next = NULL;

		{
			autolock<PQeue_write_t> al;
			this->tail->next = qn;
			this->tail = qn;
		}

		AtomicAdd(&(this->size), 1);
		return 0;
	}

	struct PQeue_read_t {};
	int front(Type &t)
	{
		QueueNode * p = NULL;
		{
			autolock<PQeue_read_t> al;

			p = this->head.next;

			if (this->tail == p) {
				return -1;
			}

			if (p == NULL) {
				return -1;
			}
			this->head.next = p->next;
		}

		t = p->val;

		delete p;

		AtomicSub(&(this->size), 1);

		return 0;
	}

private:
	// head  does not store value
	QueueNode head;
	volatile QueueNode  * tail;
	volatile uint32_t size;
   	volatile uint32_t length;
   	volatile bool flushed;
};

}
#endif
