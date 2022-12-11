#include "parser/elfParser.h"

#include <fstream>
#include <filesystem>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex>
#include <iostream>

#include "log/log.h"

namespace fs = std::filesystem;

namespace Parser
{
ElfParser::ElfParser(const char* outDir) : m_outDir(outDir)
{}

ElfParser::~ElfParser()
{}

void 
ElfParser::setSrc(const char* src)
{
    m_src = src;
}

const char* const 
ElfParser::getSrc(void) const
{
    return m_src;
}

bool
ElfParser::parse(void)
{
    if(!m_src || !m_outDir) {
        LOG::Warning("Source or out dir is not set!");
        return false;
    }

    if(!listFiles(m_listFiles, m_src)) {
        return false;
    }

    if(!generateReadElf(m_outDir)) {
        return false;
    }

    if(!listFiles(m_listReadElfFiles, m_outDir, READELF_EXTENSION)) {
        return false;
    }

    if(!parseReadElf(m_listReadElfFiles, m_parsedReadElf)) {
        return false;
    }

        
    return true;
}

const fileList& 
ElfParser::getListFiles() const
{
    return m_listFiles;
}

const fileList&
ElfParser::getListReadElFiles() const
{
    return m_listReadElfFiles;
}

const fileListExt&
ElfParser::getParsedReadElf() const
{
    return m_parsedReadElf;
}

bool
ElfParser::listFiles(fileList& list, const char* const path, const std::string& ext)
{
    for (auto &p : fs::recursive_directory_iterator(path))
    {
        if (p.path().extension() == ext) {
            
            const std::string& file =  p.path().stem().string();
            const bool slash = path[strlen(path) - 1] == '/';

            std::string filePath(path);
            if (slash) {
                filePath.append(file).append(ext);
            } else {
                filePath.append("/").append(file).append(ext);
            }

            list.insert(std::make_pair(file, filePath));
        }
    }

    return true;
}

bool 
ElfParser::generateReadElf(const char* const path)
{
    if(!dirExists(path)) {
        LOG::Error(std::string("Directory: " + std::string(path) + " does not exist!").c_str());
        return false;
    }

    for (auto const& [key, val] : m_listFiles)
    {
        std::string cmd("readelf -s --wide ");
        cmd.append(val);
        cmd.append(" > ");
        cmd.append(path);
        cmd.append(key);
        cmd.append(READELF_EXTENSION);

        if(system(cmd.c_str()) != 0) {
            LOG::Error(std::string(std::string(__PRETTY_FUNCTION__) + "Failed to generate readelf for: " + std::string(val)).c_str());
            return false;
        }
    }

    return true;
}

bool
ElfParser::parseReadElf(const fileList& srcList, fileListExt& target)
{
    for (auto const& [key, file] : srcList)
    {
        fileList pairs;

        std::ifstream readFile(file);

        //      2: 00008000     4 OBJECT  LOCAL  DEFAULT    1 print_string
        //const std::regex strExpr("^*\\d:( )?[0-9A-Fa-f]{8}*[0-9]{1,4}*\\w+*\\w+");
        //const std::regex strExpr("\\s*\\d+:\\s+(\\w+)\\s+\\d+\\s+\\w+\\s+\\w+\\s+\\w+\\s+\\d+\\s+(\\w+)");
        const std::regex strExpr("\\s*\\d+:\\s+([0-9A-Fa-f]{8})\\s+[0-9]{1,4}\\s+\\w+\\s+\\w+\\s+\\w+\\s+\\d+\\s+(\\w+)");
        std::smatch match;

        std::string data;
        while (std::getline(readFile, data)) {
            //LOG::Debug(data.c_str());
            if(std::regex_match(data, match, strExpr)) {
                pairs.insert(std::make_pair(match[2], match[1]));
            }
        }

        if(!pairs.empty()) {
            target.insert(std::make_pair(key, pairs));
        }
    }

    return true;
}

bool 
ElfParser::dirExists(const char* const path) const
{
    struct stat info;

    int statRC = stat( path, &info );
    if( statRC != 0 )
    {
        if (errno == ENOENT)  { return false; } // something along the path does not exist
        if (errno == ENOTDIR) { return false; } // something in path prefix is not a dir
        return false; // -1
    }

    return ( info.st_mode & S_IFDIR ) ? true : false;
}
}