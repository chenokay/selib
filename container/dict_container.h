
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

#ifndef  SELIB_DICT_CONTAINER
#define  SELIB_DICT_CONTAINER

#include "pointer_switcher.h"
#include "file_watcher.h"
#include "path.h"
#include "timer.h"

namespace SELIB_UTIL {

template<typename TYPE>
class FileStatus {
	public:
    typedef PointerFlipper<TYPE> pf_t;
    pf_t _bp;
    FileWatcher _fw;
};

// dict container, whch can hold all kinds of struct and 
// a specification application only need to care "load"
// and "release" method, unify the dict interface and 
// manage the process of data usage
//
// sample case in test/test_dict.cpp, the details are:
//
//
//  typedef std::map< int, int > map_t;
//  class TestDict : public DictContainer<std::map< int, int > > {
//  	public:
//  
//  		virtual ~TestDict() 
//  		{
//  			release_all();
//  		}
//  
//  		virtual type_t* load(const char* full_path)
//  		{
//  			static int count = 0 ;
//  			map_t * m = new (std::nothrow) map_t;
//  
//  			if (0 == count) {
//  				(*m)[2] = 4;
//  				(*m)[5] = 6;
//  			} else {
//  				(*m)[2] = 5;
//  				(*m)[5] = 7;
//  			} 
//  
//  			++ count;
//  
//  			return m;
//  		}
//  
//  		virtual int release(type_t* dict_handle)
//  		{
//  			delete dict_handle;
//  
//  			return 0;
//  		}
//  };
//  
//  int main()
//  {
//  	TestDict td;
//  	td.init(".", "test_file");
//  
//  	map_t * mt = td.get_dict();
//  
//  	printf("v:%d; v:%d\n", (*mt)[2], (*mt)[5]);
//  
//  	sleep(5);
//  
//  	td.reload();
//  
//  	mt = td.get_dict();
//  
//  	printf("v:%d; v:%d\n", (*mt)[2], (*mt)[5]);
//  
//  	return 0;
//  }
//  

template<typename TYPE, typename LOADPARAM = int >
class DictContainer {
public:

    typedef TYPE type_t;
    typedef LOADPARAM param_t;
    typedef FileStatus<type_t> file_status_t;

    DictContainer() 
    {
        _has_para = false;
        _initialized = false;
    }

    virtual ~DictContainer() 
    {
    }

    virtual int init(const char * file, void * para = NULL) 
    {
        if (NULL == file) {
			SELIB_WRITE_FATAL_LOG("file path is NULL!");
			return -1;
        }

		if (NULL != para) {
			_para = *(reinterpret_cast<LOADPARAM*>(para));
			_has_para = true;
		}

		_file = file;

		type_t * dict = load(file);
		if (NULL == dict) {
			SELIB_WRITE_FATAL_LOG("fail to load dict[%s]", file);
			return -1;
		}

		// get new handle when reload successfully
		_fs._bp.create(dict, NULL);
		// monitor file to reload
		_fs._fw.create(_file.c_str());

		_initialized = true;

		return 0;
    }

    virtual int init(const char* path, const char* name, void * para = NULL) 
    {
        if(path == NULL || name == NULL) {
			SELIB_WRITE_FATAL_LOG("file path is NULL!");
			return -1;
        }

		std::string file = path_join(path, name);
		return init(file.c_str(), para);

    }

    int reload() 
    {

		if (_fs._fw.check_and_update_timestamp()) {
			Timer timer;

			type_t * b_f = load(_file.c_str());

			SELIB_WRITE_WARN_LOG("reload dict time[%s][%ld]", 
					_file.c_str(), timer.sec_elapsed());

			if(NULL == b_f) {
				SELIB_WRITE_FATAL_LOG("fail to load dict data[%s]", _file.c_str());
				return -1;
			}

			// set back pointer and flip
			_fs._bp.set_back_pointer(b_f);
			_fs._bp.flip();
			SELIB_WRITE_WARN_LOG("reload dict [%s] sucessfully", _file.c_str());

		}

		return 0;
    }

    virtual type_t * get_dict()
    {
		return _initialized ? _fs._bp.fore_pointer() : NULL;
	}

	virtual type_t * get_back_dict()
	{
		return _initialized ? _fs._bp.back_pointer() : NULL;
	}

protected:
    
	// fill following two functions in derived class 
	// coresspondingly, user only care how to load 
	// and release his data
    virtual type_t* load(const char* full_path) = 0;
    virtual int release(type_t* dict_handle) = 0;

	// call the release function in derived class 
	// because pure virtual function-release can't be called in 
	// destructor
	virtual void release_all() 
	{
		if(NULL != _fs._bp.fore_pointer()) {
			release(_fs._bp.fore_pointer());
		}

		if (NULL != _fs._bp.back_pointer()) {
			release(_fs._bp.back_pointer());
		}
	}

	file_status_t _fs;
	std::string _file;
    // parameters for dict loading, include bucket size etc.
    param_t _para;  
    bool _has_para;

    bool _initialized;
};

}
#endif 
