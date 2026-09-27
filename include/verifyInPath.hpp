#pragma once
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

#ifdef _WIN32
    static const char PATH_SEP=';';
    static const char DIR_SEP='\\';
    #include <io.h>
    #include <sys/stat.h>
    static bool isRegularFile(const char* path) {
        struct _stat st;
        if(_stat(path,&st)!=0) {
            return false;
        }
        return (st.st_mode&_S_IFREG)!=0;
    }
    #define ACCESS(path) (isRegularFile(path)?0:-1)
#else
    static const char PATH_SEP=':';
    static const char DIR_SEP='/';
    #include <unistd.h>
    #include <sys/stat.h>
    static bool isRegularFile(const char* path) {
        struct stat st;
        if(stat(path,&st)!=0) {
            return false;
        }
        if(!S_ISREG(st.st_mode)) {
            return false;
        }
        return access(path,X_OK)==0;
    }
    #define ACCESS(path) (isRegularFile(path)?0:-1)
#endif

inline bool verifyInPath(const string& program) {
    if(program.empty()) {
        return false;
    }
#ifdef _WIN32
    if(program.find('/')!=string::npos||program.find('\\')!=string::npos) {
        return ACCESS(program.c_str())==0;
    }
#else
    if(program.find('/')!=string::npos) {
        return ACCESS(program.c_str())==0;
    }
#endif
    const char* path_env=getenv("PATH");
    if(!path_env) {
        return false;
    }
    istringstream path_stream(path_env);
    string dir;
    while(getline(path_stream,dir,PATH_SEP)) {
        if(dir.size()>=2&&dir.front()=='"'&&dir.back()=='"') {
            dir=dir.substr(1,dir.size()-2);
        }
        if(dir.empty()) {
            continue;
        }
        while(dir.size()>1&&(dir.back()=='/'||dir.back()=='\\')) {
            dir.pop_back();
        }
#ifdef _WIN32
        bool has_ext=false;
        size_t dot=program.find_last_of('.');
        size_t slash=program.find_last_of("/\\");
        if(dot!=string::npos&&(slash==string::npos||dot>slash)) {
            has_ext=true;
        }
        if(!has_ext) {
            const char* pathext=getenv("PATHEXT");
            string exts=pathext?pathext:".COM;.EXE;.BAT;.CMD";
            istringstream ext_stream(exts);
            string ext;
            while(getline(ext_stream,ext,';')) {
                if(ext.empty()) {
                    continue;
                }
                string full_path=dir+DIR_SEP+program+ext;
                if(ACCESS(full_path.c_str())==0) {
                    return true;
                }
            }
        }
        string full_path=dir+DIR_SEP+program;
        if(ACCESS(full_path.c_str())==0) {
            return true;
        }
#else
        string full_path=dir+DIR_SEP+program;
        if(ACCESS(full_path.c_str())==0) {
            return true;
        }
#endif
    }
    return false;
}
