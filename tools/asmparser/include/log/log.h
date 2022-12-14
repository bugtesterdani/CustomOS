
class LOG
{
public:
    LOG() = delete;
    ~LOG() = delete;

    /**
     * @brief Writes error message to console
     */
    static void Error(const char*);

    /**
     * @brief Writes warning message to console
     */
    static void Warning(const char*);

    /**
     * @brief Writes info message to console
     */
    static void Info(const char*);

    /**
     * @brief Writes log message to console
     */
    static void Log(const char*);

    /**
     * @brief Writes debug message to console
     */
    static void Debug(const char*);
};