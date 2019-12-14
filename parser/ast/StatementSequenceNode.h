#pragma once
#include "Node.h"
#include "StatementNode.h"
#include <vector>

class StatementSequenceNode : public Node {
    std::vector<std::unique_ptr<const StatementNode>> body_;

  public:
    StatementSequenceNode(const FilePos& pos);
    ~StatementSequenceNode() override;

    void pushStatement(const StatementNode* statement);

    const std::vector<std::unique_ptr<const StatementNode>>& getBody() const;
    void print(std::ostream& stream) const override;
};
