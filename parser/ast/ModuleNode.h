#pragma once
#include "DeclarationsNode.h"
#include "Node.h"
#include "StatementSequenceNode.h"

class ModuleNode : public Node {
    std::string name_;
    std::unique_ptr<const DeclarationsNode> declarations_;
    std::unique_ptr<const StatementSequenceNode> statements_;

  public:
    ModuleNode(const FilePos& pos,
                      std::string name,
                      const DeclarationsNode* declarations,
                      const StatementSequenceNode* statements);
    ~ModuleNode() override;

    void print(std::ostream& stream) const override;
};
