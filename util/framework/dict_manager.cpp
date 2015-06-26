
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

#include <stdint.h>
#include "dict_manager.h"

namespace SELIB_UTIL {
static const uint32_t DICT_NUM = 100;

int DictManager::dict_process(proc_type action)
{
    uint64_t dict_count = _dict_key.size();

	for (uint32_t i = 0; i < dict_count;  ++ i) {

		std::string & key = _dict_key[i];
		dict_info_t  * di = NULL;
		std::map<std::string, dict_info_t*>::iterator iter;

		if (_index2dict.end() != (iter = _index2dict.find(key))) {
			di = iter->second;

			if (!di->_valid) {
				continue;
			}
			di->_action->process(action, *di);
			if (DEL == action) {
				delete di->_action;
				delete di;
			}

		} else {
			SELIB_WRITE_FATAL_LOG("dict not exist[%s]", key.c_str());
		}
	}

	return 0;
}

int DictManager::load()
{
	int ret = 0;

	ret = dict_process(LOAD);
	if (0 != ret) {
        return ret;
    }
    _loaded = true;
    return 0;
}

int DictManager::reload()
{
    if (!_loaded) {
        SELIB_WRITE_FATAL_LOG("dict is not loaded");
        return -1;
    }

	return dict_process(RELOAD);

}

int DictManager::destroy()
{

    if (_loaded) {
        dict_process(DEL);
    }

    _loaded = false;
    return 0;
}

}

