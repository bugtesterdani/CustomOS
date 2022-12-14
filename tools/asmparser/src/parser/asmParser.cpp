#include "parser/asmParser.h"

#include "log/log.h"

#include <regex>
#include <cstring>

namespace Parser 
{
ASMParser::ASMParser(const fileListExt& parsedReadElf) : 
    m_parsedReadElf(parsedReadElf)
{}
    
ASMParser::~ASMParser()
{}

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

    if(!parseAsm(m_parsedReadElf, m_outputFile, m_src)) {
        LOG::Error(std::string("Failed to parse asm!").c_str());
        return false;
    }

    if(!writeFile(m_outputFile, m_src)) {
        LOG::Error("Failed to write into new file!");
        return false;
    }

    LOG::Info("Parse of .asm files done!");
    return true;;
}

bool
ASMParser::parseAsm(const fileListExt& list, std::vector<std::string>& output, const char* const file)
{
    std::ifstream readFile(file);

    //[print_string]
    const std::regex strExprFunc(".*[$](\\w+)\\.(\\w+).*");
    const std::regex strExprPoint(".*[*](\\w+)\\.(\\w+).*");
    std::smatch match;
    std::string data;
    std::string newData;
    uint32_t lineCount = 0;
    uint32_t posCount = 0;

    
    while (std::getline(readFile, data)) {
        lineCount++;
        newData.clear();

        Option option = NONE;
        if(std::regex_match(data, match, strExprFunc)) {
            option = FUNCTION;
        } else if (std::regex_match(data, match, strExprPoint)) {
            option = POINTER;
        } else {
            output.push_back(data);
            continue;
        }

        const std::string key = match[1];
        const std::string func = match[2];

        if(!doesKeyExist(key, func)) {
            output.push_back(data);
            continue;
        }

        const std::string address = list.find(key)->second.find(func)->second;

        if(option == ASMParser::FUNCTION) {
            for(int i = 0; i < data.length(); i++) {
                const char c = data.at(i);
                if(c == DOLLAR) {
                    if (data.at(i-1) == SQUARE_BRACKET_OPEN) {
                        newData.pop_back();
                    } else {
                        LOG::Warning(std::string("($) Missing '[' at line: " + std::to_string(lineCount) + ", pos: " + std::to_string(i+1)).c_str());
                    }

                    std::for_each(address.begin(), address.end(), [&newData] (const char v) {
                        newData.push_back(v);
                    });

                    i += sizeof(DOLLAR) + key.length() + func.length();
                   
                    if((i + 1) < data.length()) {
                        if(data.at(i + 1) == SQUARE_BRACKET_CLOSED) {
                            i++;
                        } else {
                            LOG::Warning(std::string("($) Missing ']' at line: " + std::to_string(lineCount) + ", pos: " + std::to_string(i+2)).c_str());
                        }
                    } else {
                        LOG::Warning(std::string("($) Missing ']' at line: " + std::to_string(lineCount) + ", pos: " + std::to_string(i+2)).c_str());
                    }
                } else {
                    newData.push_back(c);
                }
            }
            m_outputFile.push_back(newData);
        } else if(option == ASMParser::POINTER) {
            for(int i = 0; i < data.length(); i++) {
                const char c = data.at(i);
                if(c == ASTERIKS) {
                    if (data.at(i-1) != SQUARE_BRACKET_OPEN) {
                        newData.push_back(SQUARE_BRACKET_OPEN);
                        LOG::Warning(std::string("(*) Missing '[' at line: " + std::to_string(lineCount) + ", pos: " + std::to_string(i + 1)).c_str());
                    } 

                    std::for_each(address.begin(), address.end(), [&newData] (const char v) {
                        newData.push_back(v);
                    });

                    i += sizeof(ASTERIKS) + key.length() + func.length();
                    
                    if ((i+1) < data.length()) {
                        if (data.at(i + 1) != SQUARE_BRACKET_CLOSED) {
                            newData.push_back(SQUARE_BRACKET_CLOSED);
                            LOG::Warning(std::string("(*) Missing ']' at line: " + std::to_string(lineCount) + ", pos: " + std::to_string(i + 2)).c_str());
                        }
                    } else {
                        newData.push_back(SQUARE_BRACKET_CLOSED);
                        LOG::Warning(std::string("(*) Missing ']' at line: " + std::to_string(lineCount) + ", pos: " + std::to_string(i + 2)).c_str());
                    }
                } else {
                    newData.push_back(c);
                }
            }
            m_outputFile.push_back(newData);
        } else {
            m_outputFile.push_back(data);
        }
    }
    return true;
}

bool
ASMParser::doesKeyExist(const std::string& mainKey, const std::string& primKey)
{
    fileListExt::const_iterator pos = m_parsedReadElf.find(mainKey);
    if (pos == m_parsedReadElf.end()) {
        LOG::Warning(std::string("Key: '" + mainKey + "' does not exist").c_str());
        return false;
    } else if(!primKey.empty()) {
        if (pos->second.find(primKey) == pos->second.end()) {
            LOG::Warning(std::string("Func key: '" + primKey + "' does not exist").c_str());
            return false;
        }
    }
    return true;
}

void 
ASMParser::printKeys(void)
{
    LOG::Info("Existing keys:");
    for(auto const& [key, val] : m_parsedReadElf) {
        for (auto const& [key1, val1] : val) {
            LOG::Info(std::string(key + ": " + key1).c_str());
        }
    }
}

bool
ASMParser::writeFile(const std::vector<std::string>& list, const char* newFile)
{
    std::string outFile = m_outDir;

    std::size_t size = outFile.length();
    if(outFile.at(size -1) != '/') {
        outFile.push_back('/');
    }

    int lastIndex;
    for(int s = 0; s < strlen(newFile); s++) {
        if (newFile[s] == '/') {
            lastIndex = s + 1;
        }
    }

    for(int o = lastIndex; o < strlen(newFile); o++) {
        outFile.push_back(newFile[o]);
    }

    std::ofstream file(outFile.c_str(), std::ofstream::out);

    if(file.is_open())
    {
        const size_t size = list.size();
        for(int i = 0; i < size; i++) {
            file << list.at(i);
            if(i != size - 1) {
                file << std::endl;
            }
        }
        file.close();
        return true;
    }
    return false;
}
}
