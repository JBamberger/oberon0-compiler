#pragma once
#include "Node.h"

class TypeNode : public Node {
  public:
    TypeNode(NodeType nodeType, const FilePos& pos) : Node(nodeType, pos) {}
};