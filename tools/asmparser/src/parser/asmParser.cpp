#include "parser/asmParser.h"

#include "log/log.h"

#include <regex>

namespace Parser 
{
ASMParser::ASMParser(const fileListExt& parsedReadElf) : 
    m_parsedReadElf(parsedReadElf)
{}
    
ASMParser::~ASMParser()
{
    LOG::Info("Delete ASMParser!");
}

void
ASMParser::setSrc(const char* src)
{
    m_src = src;
}

const char* const
ASMParser::getSrc(void) const
{
    return m_src;
}

void
ASMParser::setOutDir(const char* outDir)
{
    m_outDir = outDir;
}

const char* const
ASMParser::getOutDir(void) const
{
    return m_outDir;
}

bool 
ASMParser::parse(void)
{
    if(!m_src || !m_outDir) {
        LOG::Warning("Source or out dir is not set!");
        return false;
    }

    if(!dirExists(m_outDir)) {
        LOG::Error(std::string("Directory: " + std::string(m_outDir) + " does not exist!").c_str());
        return false;
    }

    if(!fileExists(m_src)) {
        LOG::Error(std::string("File: " + std::string(m_src) + " does not exist!").c_str());
        return false;
    }

    std::ifstream readFile(m_src);

    //    call dword [print_string]
    const std::regex strExpr("");
    std::smatch match;

    std::string data;
    while (std::getline(readFile, data)) {
        LOG::Debug(data.c_str());
        if(std::regex_match(data, match, strExpr)) {
            
        }
    }

    LOG::Info("Parse of .asm files done!");
    return true;
}
}