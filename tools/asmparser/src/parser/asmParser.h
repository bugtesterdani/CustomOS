#include "parser/parseBase.h"

#include <vector>

#define DOLLAR '$'
#define ASTERIKS '*'
#define SQUARE_BRACKET_OPEN '['
#define SQUARE_BRACKET_CLOSED ']'

namespace Parser
{
class ASMParser : public ParseBase {
public:
    ASMParser(const fileListExt&);
    ~ASMParser();

    void setSrc(const char*) override;

    const char* const getSrc(void) const override;

    void setOutDir(const char*) override;

    const char* const getOutDir(void) const override;

    bool parse(void);

private:
    bool parseAsm(const fileListExt&, std::vector<std::string>&, const char* const);
    bool doesKeyExist(const std::string&, const std::string& = std::string());
    void printKeys(void);
    bool writeFile(const std::vector<std::string>&, const char*);

private:
    const fileListExt& m_parsedReadElf;
    fileList ok;
    std::vector<std::string> m_outputFile;
    enum Option {
        NONE,
        FUNCTION, // call dword [$stage1.print_string]
        POINTER   // call dword [*stage1.print_string]
    };
};
}