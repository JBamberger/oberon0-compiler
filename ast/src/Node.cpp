#include "Node.h"
#include <utility>

Node::Node(FilePos pos) : pos_(std::move(pos))
{
}

Node::~Node() = default;

FilePos Node::getFilePos() const
{
    return pos_;
}

std::ostream& operator<<(std::ostream& stream, const Node& node)
{
    node.print(stream);
    return stream;
}
