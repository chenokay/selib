
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

#ifndef SELIB_PATH
#define SELIB_PATH

#include <string.h>

namespace SELIB_UTIL {

// Split the path name to directory and file name
// @return true if success
// e.g. path="/foo/bar/a" ==> dir="/foo/bar/", name="a"
//      path="foo"        ==> dir=".",         name="foo"
inline bool path_split(const char *path, std::string *dir, std::string *name)
{
    if (path == NULL) {
        return false;
    }

    const char *base = NULL;
    char *sep = const_cast<char*>(strrchr(path, '/'));
    if (sep == NULL) {
        // no separator
        base = path;
    } else {
        base = sep + 1;
    }

    if (dir) {
        dir->clear();

        if (path == base) {
            dir->append(".");
        } else {
            dir->append(path, base - path);
        }
    }

    if (name) {
        name->clear();
        name->append(base);
    }
    
    return true;
}

// join path
// e.g. dir="/foo/bar/", child="a"     ==> return "/foo/bar/a"
//      dir="foo",       child="/bar"  ==> return "/bar"
//      dir="foo",       child="bar"   ==> return "foo/bar"
inline std::string path_join(const char*dir, const char*child)
{
    std::string path;
    if (dir != NULL) {
        path = dir;
    }
    size_t s = path.size();

    if (child != NULL && child[0] == '/') {
        path = child;
    } else if (s == 0 || dir[s - 1] == '/') {
        path.append(child);
    } else if (child != NULL){
        path.append("/");
        path.append(child);
    }
    return path;
}

} 

#endif 
