/*
 * Header file of the base class of all AST nodes used by the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 2/2/18.
 */

#ifndef OBERON0C_AST_H
#define OBERON0C_AST_H

#include "Logger.h"
#include <memory>
#include <ostream>

class NodeVisitor;

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

#endif // OBERON0C_AST_H