#pragma once
#include "Node.h"

class TypeNode : public Node {
  public:
    TypeNode() : Node(NodeType::type, FilePos()) {}

    /**
     * Get a string representation of the type.
     * @return string identifying this type
     */
    virtual std::string getId() const = 0;

    /**
     * The size of this type in memory in bytes.
     * @return size in bytes
     */
    virtual size_t getByteSize() const = 0;
};