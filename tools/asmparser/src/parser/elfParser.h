#include "parser/parseBase.h"

#include <map>
#include <string>

using fileList = std::map<std::string, std::string>;
using fileListExt = std::map<std::string, std::map<std::string, std::string>>;

#define READELF_EXTENSION ".readelf"

namespace Parser
{
class ElfParser : public ParseBase {
public:
    ElfParser(const char*);
    ~ElfParser();

    void setSrc(const char*) override;

    const char* const getSrc(void) const override;

    bool parse(void);

    const fileList& getListFiles() const;

    const fileList& getListReadElFiles() const;

    const fileListExt& getParsedReadElf() const;

private:
    /**
     * @brief Get all files and list them
     * @return true if successful else false
    */
    bool listFiles(fileList& list, const char* const path, const std::string& ext = ".elf");

    bool generateReadElf(const char* const);

    bool parseReadElf(const fileList& srcList, fileListExt& target);

    bool dirExists(const char* const) const;

private:
    const char* m_outDir;
    fileList m_listFiles; // Input elf files for generation of readelf files
    fileList m_listReadElfFiles; // Generatet readelf files
    fileListExt m_parsedReadElf; // Parsed readelf file
};
}