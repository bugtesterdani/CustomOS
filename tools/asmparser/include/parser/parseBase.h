#pragma once

#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>

using fileList = std::map<std::string, std::string>;
using fileListExt = std::map<std::string, std::map<std::string, std::string>>;
namespace fs = std::filesystem;

class ParseBase {
public:
    ParseBase() : m_src(nullptr) {};
    virtual ~ParseBase() {}

    virtual void setSrc(const char*) = 0;

    virtual const char* const getSrc(void) const = 0;

    virtual void setOutDir(const char*) = 0;

    virtual const char* const getOutDir(void) const = 0;

protected:
    bool dirExists(const char* const path) const {
        struct stat info;

        int statRC = stat(path, &info );
        if( statRC != 0 )
        {
            if (errno == ENOENT)  { return false; } // something along the path does not exist
            if (errno == ENOTDIR) { return false; } // something in path prefix is not a dir
            return false; // -1
        }

        return ( info.st_mode & S_IFDIR ) ? true : false;
    }

    bool fileExists(const char* file) const {
        struct stat buffer;   
        return (stat (file, &buffer) == 0); 
    }

protected:
    const char* m_src;
    const char* m_outDir;
};