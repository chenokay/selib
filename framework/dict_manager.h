
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

#ifndef SELIB_DICT_MANAGER
#define SELIB_DICT_MANAGER

#include <deque>
#include <map>
#include <string>
#include "log_print.h"
#include "path.h"
#include "singleton.h"

namespace SELIB_UTIL {

class DictManager {
    // to add a dict into DictManager, all what
    // should do is definitions:
    //     // 1. call set_dict_property, which set dict key,
    //           dict type, valid flag and reload flag
    //     // 2. call get_dict_handle to get dict handle
	//
	//     sample case in test/test_dict_manager.cpp and the mainly code is:
	//
	//	
	// SELIB_STL_CPP::DictManager & inst=	SELIB_STL_CPP::DictManager::instance();
	//	 // init the dictionary property
	// inst.set_dict_property<TestDict>("test_dict", "./test_file", true, true);
	//   // load, reload can be put in one new thread
	// inst.load();
	//   // use
	// TestDict * td = inst.get_dict_handle<TestDict>("test_dict");

    friend class ProcessSingleton<DictManager>;

	// built-in structure
private:
	enum proc_type {
		LOAD,
        RELOAD,
        DEL
    };
    class IAction;

    struct dict_info_t {
        std::string _path;
        bool _with_reload;
        void * _dict_instance;
        IAction * _action;
        bool _valid;
    };

	// store actions of different type
	// and call in same method
    class IAction {
    public:
        IAction() {}
        virtual ~ IAction() {}
        virtual int process(proc_type & action, dict_info_t & di) = 0;
    };

    template <typename T, typename _PARAM = int >
    class ActionInstance : public IAction {
    public:
        ActionInstance(T * inst, int (T::*init)(const char *, void *),
                int (T::*reload)(), void * par = NULL) : _dict(inst),
            _init(init), _reload(reload) {

				if (NULL != par) {
					_par = *(reinterpret_cast<_PARAM*>(par));;
					_has_par = true;
				} else {
					_has_par = false;
				}

        }
        virtual ~ ActionInstance() {}
        virtual int process(proc_type & action, dict_info_t & di) {
            if (LOAD == action) {
                if (NULL == _dict) {
                    SELIB_WRITE_FATAL_LOG("dict handle is NULL[%s]", di._path.c_str());
                    return -1;
                }
                if (_has_par) {
                    (_dict->*_init)(di._path.c_str(), &_par);
                } else {
                    (_dict->*_init)(di._path.c_str(), NULL);
                }
            } else if (RELOAD == action) {
                if (di._with_reload) {
                    (_dict->*_reload)();
                }
            } else if (DEL == action) {
                delete _dict;
            } else {
                SELIB_WRITE_FATAL_LOG("process type wrong[%d]", action);
                return -1;
            }
            return 0;
        }

    private:
        T * _dict;
        int (T::*_init)(const char *, void *);
        int (T::*_reload)();
        _PARAM _par;
        bool _has_par;
    };

public:
    // for singleton method
    static DictManager & instance() {
        return *(ProcessSingleton<DictManager>::get());
    }

    // set the properties of dict
    // include: path; reloading flag and valid flag
	//
	// key : a string description for a class has a few instances
	// file : file path
	// with_reload : if the dictionary will be reladed
	// valid : a flag to control the data cater to the developing process
	// par : paramter coressponding to load 
    template<typename TYPE>
    int set_dict_property(const char * key, 
			const std::string& file,
            bool with_reload, 
			bool valid, 
			void * par = NULL) {
        DictManager & dict_pool = DictManager::instance();
        DictManager::dict_info_t * di_info = dict_pool.init_dict_property(key, file.c_str(), with_reload, valid);
        if (NULL != di_info) {
            di_info->_dict_instance = new(std::nothrow) TYPE;
            if (NULL == di_info->_dict_instance) {
                SELIB_WRITE_FATAL_LOG("fail to new dict[%s]", key);
                return -1;
            }

			// store the instance and method address, so that it can be
			// operated only by readable key
            di_info->_action = new(std::nothrow) ActionInstance<TYPE>((TYPE*)di_info->_dict_instance,
                    &TYPE::init, &TYPE::reload, par);

            if (NULL == di_info->_action) {
                SELIB_WRITE_FATAL_LOG("new ActionInstance fail");
                return -1;
            }

        } else {
            SELIB_WRITE_FATAL_LOG("init_dict_property fail, return NULL");
            return -1;
        }
        return 0;
    }

    template<typename TYPE>
    int set_dict_property(const char * key, const std::string& path,
            const std::string& name, bool with_reload, bool valid, void * par = NULL) {
        std::string file = path_join(path.c_str(), name.c_str());
        return set_dict_property<TYPE>(key, file, with_reload, valid, par);
    }

	// get the dict handle by type and key
	// make a gread attention to the key which can't
	// be repeated for different instance
    template<typename TYPE>
    TYPE * get_dict_handle(const char * key) {

		std::map<std::string, dict_info_t*>::iterator iter;
        if (_index2dict.end() != (iter = _index2dict.find(key))) {

            if (NULL == iter->second->_dict_instance) {
                SELIB_WRITE_FATAL_LOG("dict pointer is null!");
                return NULL;
            }

            return reinterpret_cast<TYPE *>(iter->second->_dict_instance);

        } else {
            SELIB_WRITE_FATAL_LOG("can't find dict! idx:%s", key);
            return NULL;
        }
    }

    int load();
    int reload();

private:
    // only for singeton use
    DictManager():  _loaded(false) {}
    ~DictManager() {
        destroy();
    }

    int dict_process(proc_type action);

	dict_info_t * init_dict_property(const std::string key, 
			const std::string path,
			const bool with_reload, 
			bool valid) {

		dict_info_t * di = new(std::nothrow)dict_info_t;
		if (NULL == di) {
			SELIB_WRITE_FATAL_LOG("new dict_info struct fail");
			return NULL;
		}

		di->_path = path;
		di->_with_reload = with_reload;
		di->_valid = valid;
		_index2dict[key] =  di;
		_dict_key.push_back(key);
		return di;
	}

    int destroy();
	// disallow copy
	DictManager(const DictManager&);
	void operator=(DictManager&);

	

	std::map<std::string, dict_info_t*> _index2dict;
    std::deque<std::string> _dict_key;
    bool _loaded;
};

template<typename CONTAINER_TYPE, typename INNER_TYPE>
INNER_TYPE * get_dict(const char * key)
{
	DictManager & dm = SELIB_STL_CPP::DictManager::instance();
	CONTAINER_TYPE * _pc = dm.get_dict_handle<CONTAINER_TYPE>(key);
	if (NULL == _pc) {
		return NULL;
	}
	return _pc->get_dict();
}

}

#endif
