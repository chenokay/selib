// a light-weight LRU cache which store value in circle
// list buffer, and find value in open address hash

#ifndef CIRCLE_CACHE_H
#define CIRCLE_CACHE_H

#include <new>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string>

namespace SELIB_UTIL {
template<class VALUE>
class LruCache {
public:
typedef uint64_t key_t;
typedef VALUE value_t;
static const uint32_t INVALID_INDEX = 0xFFFFFFFF;

LruCache() {}
~LruCache() {}
int init(uint32_t capacity, uint32_t max_probing) 
{
	_max_probing = max_probing;
	// two times bucket size for cache list capacity
	uint32_t hs = capacity * 2;
	// use pow of 2 as bucket size in order to compute 
	// hash probing position
	_hash_size = static_cast<uint32_t>(pow(2, get_bit_pos(hs) + 1));

	_hash = new(std::nothrow) hash_node_t[_hash_size];
	if (_hash == NULL) {
		return -1;
	}

	_capacity = capacity;
	_list = new(std::nothrow) list_node_t[_capacity];
	if (_list == NULL) {
		return -1;
	}

	// begin with 1
	_head  = 0;
	_tail = 0;

	// make circle buffer
	_list[0].next = 1;
	_list[0].last = _capacity - 1;

	for (uint32_t i = 1; i < _capacity - 1; ++ i) {
		_list[i].next = i + 1;
		_list[i].last = i - 1;
	}

	_list[_capacity - 1].next = 0;
	_list[_capacity - 1].last = _capacity - 2;

	return 0;
}

// if return is not null
// destruct value buffer
// use raw_key when application must have no collision 
int put(key_t k, value_t * v, std::string& raw_key = "", bool overwrite = true)
{
	uint32_t bucket = put_probing_bucket(k, raw_key, overwrite);
	if (bucket == INVALID_INDEX || bucket >= _hash_size) {
		return -1;
	}

	uint32_t index = 0;
	_hash[bucket].key = k; 
	_hash[bucket].raw_key = raw_key; 

	if (_hash[bucket].index != INVALID_INDEX &&
			_list[_hash[bucket].index].key == k) {
		index = _hash[bucket].index;
	} else {
		index = _head;
	}

	_hash[bucket].index = index; 
	_list[index].key = k;
	_list[index].v = v;
	_list[index].bucket = bucket;

	if (index == _head) {
		// new node
		 write_adjust(bucket);
	} else {
		read_adjust(index);
	}

	return 0;
}

value_t * get(key_t k, std::string& raw_key = "")
{
	uint32_t bucket = get_probing_bucket(k, raw_key);
	if (bucket == INVALID_INDEX || bucket >= _hash_size) {
		return NULL;
	}

	value_t * v = _list[_hash[bucket].index].v;

	read_adjust(_hash[bucket].index);

	return v;
}

int debug_print()
{
	for (uint32_t i = _tail; i != _head; i = _list[i].next) {
		printf("value is:%lu\n", _list[i].key);
	}

	return 0;
}

private:
struct hash_node_t {
	key_t key;
	uint32_t index;
	std::string raw_key;
	hash_node_t() 
	{
		key = 0;
		raw_key = "";
		index = INVALID_INDEX;
	}
};
hash_node_t * _hash;

struct list_node_t {
	key_t key;
	uint32_t next;
	uint32_t last;
	uint32_t bucket;
	value_t * v;
	list_node_t ()
	{
		key = 0;
		next = INVALID_INDEX;
		last = INVALID_INDEX;
		bucket = INVALID_INDEX;
		v = NULL;
	}
};
list_node_t * _list;

// the node to use 
uint32_t _head;
// least recently used one
uint32_t _tail;

uint32_t _hash_size;
uint32_t _capacity;
uint32_t _max_probing;

int write_adjust(uint32_t bucket)
{
	// full, release tail node
	if (_tail == _list[_head].next) {

		_list[_tail].key = 0;
		// delete value
		_list[_tail].v = NULL;
		uint32_t tail_bucket = _list[_tail].bucket;
		// delete information in hash
		_hash[tail_bucket].key = 0;
		_hash[tail_bucket].index = INVALID_INDEX;
		_hash[tail_bucket].raw_key = "";

		_head = _tail;
		_tail = _list[_tail].next;

	} else {
		_head = _list[_head].next;
	}

	return 0;
}

int read_adjust(uint32_t index)
{
	// del red node position
	_list[_list[index].last].next = _list[index].next;
	_list[_list[index].next].last = _list[index].last;

	// insert red node before head
	_list[_list[_head].last].next = index;
	_list[index].last = _list[_head].last;

	// read current index
	// adjust tail value
	if (_tail == index && _head != _list[index].next) {
		_tail = _list[index].next;
	}

	_list[index].next = _head;
	_list[_head].last = index;

	return 0;
}

uint32_t get_probing_bucket(key_t key, std::string& raw_key)
{
	for(int i = 0; i < _hash_size && i < _max_probing; ++i) {
		uint32_t p = quadratic_probing(key, i);
		if (_hash[p].key == key && _hash[p].raw_key == raw_key) {
			return p;
		}
	}

	// return max uint32_t
	return INVALID_INDEX;
}

uint32_t put_probing_bucket(key_t key, std::string& raw_key, bool overwrite)
{
	for(int i = 0; i < _hash_size && i < _max_probing; ++i) {
		uint32_t p = quadratic_probing(key, i);
		if (_hash[p].key == 0) {
			return p;
		}
		if (_hash[p].key == key && 
				_hash[p].raw_key == raw_key && overwrite) {
			return p;
		}
	}

	// return max uint32_t
	return INVALID_INDEX;
}

uint32_t quadratic_probing(key_t key, uint32_t i)
{
	// parameter all are 0.5
	// a % b, if b is pow of 2, it's eqeual with a & (b - 1)
	uint32_t probing_offset = static_cast<uint32_t>(0.5*i + 0.5*i*i);
	return ( ((key & (_hash_size - 1)) + probing_offset ) & (_hash_size - 1) );
}

// resize the bucket to pow of 2
int get_bit_pos(uint32_t v)
{
	int r = 0;
	while (v >>= 1) // unroll for more speed...
	{
		++ r;
	}

	return r;
}
};

}

#endif
