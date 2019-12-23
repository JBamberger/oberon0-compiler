/*
 * Header file of the logger class used by the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 2/8/18.
 */
#ifndef OBERON0C_ERRORLOG_H
#define OBERON0C_ERRORLOG_H

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

struct FilePos {
    std::string fileName;
    int lineNo, charNo;

    friend std::ostream& operator<<(std::ostream& os, const FilePos& obj)
    {
        if (!obj.fileName.empty()) {
            os << obj.fileName;
            if (obj.lineNo >= 0) {
                os << ":" << obj.lineNo;
                if (obj.charNo >= 0) {
                    os << ":" << obj.charNo;
                }
            }
        }
        return os;
    }
};

enum class LogLevel : unsigned int { DEBUG = 1, INFO = 2, ERROR = 3 };

class Logger {

  private:
    LogLevel level_;
    std::ostream *out_, *err_;

    void log(LogLevel level,
             const std::string& fileName,
             int lineNo,
             int charNo,
             const std::string& msg) const;
    void log(LogLevel level, const std::string& fileName, const std::string& msg) const;

  public:
    explicit Logger();
    explicit Logger(LogLevel level, std::ostream* out, std::ostream* err);
    ~Logger();

    void error(FilePos pos, const std::string& msg) const;
    void error(const std::string& fileName, const std::string& msg) const;
    void info(const std::string& fileName, const std::string& msg) const;
    void debug(const std::string& fileName, const std::string& msg) const;

    void setLevel(LogLevel level);
};

template <typename T>
static std::string to_string(T obj)
{
    std::stringstream stream;
    stream << obj;
    return stream.str();
}

#endif // OBERON0C_ERRORLOG_H
