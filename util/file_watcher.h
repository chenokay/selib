
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
//	reference from other engineer
//

#ifndef SELIB_FILE_WATCHER
#define SELIB_FILE_WATCHER

#include <sys/time.h>
#include <sys/stat.h>
#include <string>
#include "log_print.h"

namespace SELIB_UTIL {

class FileWatcher {
public:
    
    explicit FileWatcher ()
        :  _last_ts(0)
    {}

    int create (const char* file_path)
    {
        if (NULL == file_path) {
            SELIB_WRITE_FATAL_LOG("file_path is NULL");
            return -1;
        }
        
        _file_path = file_path;

        struct stat tmp_st;
        int ret = stat (_file_path.c_str(), &tmp_st);
        if (ret < 0) {
            SELIB_WRITE_FATAL_LOG("%s does not exist", _file_path.c_str());
			return -1;
        }

		_last_ts = tmp_st.st_mtime;
		return 0;
    }

    bool check_and_update_timestamp ()
    {
        struct stat tmp_st;
        const int ret = stat (_file_path.c_str(), &tmp_st);
        if (ret < 0) {
			SELIB_WRITE_FATAL_LOG("%s does not exist", _file_path.c_str());
			return false;
		}

		if (tmp_st.st_mtime > _last_ts) {
			_last_ts = tmp_st.st_mtime;
			return true;
		} 

		_last_ts = tmp_st.st_mtime;
		return false;
    }

    const char* filepath () const { return _file_path.c_str(); }

private:

    std::string _file_path;
    time_t _last_ts;    
};

}
#endif  
