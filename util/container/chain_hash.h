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

// a very clear and lightweight chain hash

#ifndef SELIB_CHAIN_HASH_H
#define SELIB_CHAIN_HASH_H

#include <new>

namespace SELIB_UTIL {

template<class Type>
class HashEntry {
private:
	unsigned long key;
	Type value;
	HashEntry<Type> *next;
public:
	HashEntry(unsigned long key, Type value) 
	{
		this->key = key;
		this->value = value;
		this->next = NULL;
	}

	unsigned long getKey() 
	{
		return key;
	}

	Type * getValue() 
	{
		return &value;
	}

	void setValue(Type value) 
	{
		this->value = value;
	}

	HashEntry *getNext() 
	{
		return next;
	}

	void setNext(HashEntry *next) 
	{
		this->next = next;
	}
};

template<class Type>
class ChainHashMap {
typedef HashEntry<Type> LinkedHashEntry;
private:
LinkedHashEntry **table;
uint32_t _bucket_num;
public:
ChainHashMap() {}

int create(int bucket_num)
{
	_bucket_num = bucket_num;
	table = new (std::nothrow) LinkedHashEntry*[_bucket_num];
	if (NULL == table) {
		SELIB_WRITE_FATAL_LOG("fail to new hash chain");
		return -1;
	}

	for (uint32_t i = 0; i < _bucket_num; i++) {
		table[i] = NULL;
	}

	return 0;
}

Type * find(unsigned long key) {
	int hash = (key % _bucket_num);
	if (table[hash] == NULL) {
		return NULL;
	} else {
		LinkedHashEntry *entry = table[hash];
		while (entry != NULL && entry->getKey() != key) {
			entry = entry->getNext();
		}
		if (entry == NULL) {
			return NULL;
		} else {
			return entry->getValue();
		}
	}
}

bool insert(unsigned long key, Type value, bool overwritten = true) {
	int hash = (key % _bucket_num);
	if (table[hash] == NULL) {
		table[hash] = new (std::nothrow)LinkedHashEntry(key, value);
		if (NULL == table[hash]) {
			SELIB_WRITE_FATAL_LOG("fail to new hash entry");
			return false;
		}
	} else {
		LinkedHashEntry *entry = table[hash];
		while (entry->getNext() != NULL) {
			entry = entry->getNext();

			if (entry->getKey() == key) {
				if (overwritten) {
					entry->setValue(value);
				}

				return true;
			}
		}

		LinkedHashEntry * nt = new (std::nothrow) LinkedHashEntry(key, value);
		if (NULL == nt) {
			SELIB_WRITE_FATAL_LOG("fail to new hash entry");
			return false;
		}

		entry->setNext(nt);

	}

	return true;
}

void remove(unsigned long key) {
	int hash = (key % _bucket_num);
	if (table[hash] != NULL) {
		LinkedHashEntry *prevEntry = NULL;
		LinkedHashEntry *entry = table[hash];
		while (entry->getNext() != NULL && entry->getKey() != key) {
			prevEntry = entry;
			entry = entry->getNext();
		}
		if (entry->getKey() == key) {
			if (prevEntry == NULL) {
				LinkedHashEntry *nextEntry = entry->getNext();
				delete entry;
				table[hash] = nextEntry;
			} else {
				LinkedHashEntry *next = entry->getNext();
				delete entry;
				prevEntry->setNext(next);
			}
		}
	}
}

~ChainHashMap() 
{
	destroy();

	delete[] table;
}

int destroy()
{
	for (uint32_t i = 0; i < _bucket_num; i++) {
		if (table[i] != NULL) {
			LinkedHashEntry *prevEntry = NULL;
			LinkedHashEntry *entry = table[i];
			while (entry != NULL) {
				prevEntry = entry;
				entry = entry->getNext();
				delete prevEntry;
			}

		}
	}

	return 0;
}

int clear()
{
	destroy();
	for (uint32_t i = 0; i < _bucket_num; i++) {
		table[i] = NULL;
	}

	return 0;

}

};

}

#endif
