#pragma once
#include "Node.h"

class StatementNode : public Node {
  public:
    StatementNode(NodeType nodeType, const FilePos& pos) : Node(nodeType, pos)
    {
    }
};