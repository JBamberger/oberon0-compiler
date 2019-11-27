#pragma once
#include "Node.h"
#include <vector>

class IdentifierListNode : public Node {
    std::vector<std::string> names_;

  public:
    IdentifierListNode(const FilePos& pos, std::string name);
    ~IdentifierListNode() override;

    const std::vector<std::string>& getNames() const;
    void addName(std::string name);
    void print(std::ostream& stream) const override;
};
