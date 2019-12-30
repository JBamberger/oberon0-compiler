#pragma once
#include <memory>
#include <string>
#include <vector>

template <class T, class D>
std::ostream& printList(std::ostream& stream,
                        const std::string& name,
                        const std::vector<std::unique_ptr<T, D>>& list_)
{
    stream << name << "(";
    if (!list_.empty()) {
        stream << list_.at(0);
        for (size_t i = 1; i < list_.size(); ++i) {
            stream << ", " << list_.at(i);
        }
    }
    stream << ")";
    return stream;
}
