#pragma once

#include <iostream>
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