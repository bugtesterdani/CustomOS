#include "parser/parseBase.h"

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

private:
    const fileListExt& m_parsedReadElf;
    fileList ok;
};
}