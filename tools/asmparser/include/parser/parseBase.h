class ParseBase {
public:
    ParseBase() : m_src(nullptr) {};
    virtual ~ParseBase() {}

    virtual void setSrc(const char*) = 0;

    virtual const char* const getSrc(void) const = 0;

protected:
    const char* m_src;
};