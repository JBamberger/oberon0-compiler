#pragma once
#include "Node.h"

class StatementNode : public Node {
  public:
    StatementNode(const FilePos& pos) : Node(pos)
    {
    }
};