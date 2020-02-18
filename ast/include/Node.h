#pragma once

#include "Logger.h"
#include <memory>
#include <ostream>

class NodeVisitor;

/**
 * Base class for all abstract syntax tree nodes.
 */
class Node {
    FilePos pos_;

  public:
    explicit Node(FilePos pos);

    virtual ~Node() = 0;

    FilePos getFilePos() const;

    virtual void visit(NodeVisitor* visitor) const = 0;

    virtual void print(std::ostream& stream) const = 0;

    friend std::ostream& operator<<(std::ostream& stream, const Node& node);
};
