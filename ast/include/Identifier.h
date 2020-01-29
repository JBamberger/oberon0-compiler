#pragma once

#include <ostream>
#include <string>
#include <utility>

struct Identifier {
    std::string name;
    FilePos pos;

    Identifier(std::string name, FilePos pos) : name(std::move(name)), pos(std::move(pos))
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Identifier& obj)
    {
        return os << "Identifier(" << obj.name << ", " << obj.pos << ")";
    }

    friend std::string operator+(const std::string& lhs, const Identifier& rhs)
    {
        std::stringstream s;
        s << lhs << rhs;
        return s.str();
    }
};
