#include "Node.h"
#include <utility>

Node::Node(const NodeType nodeType, FilePos pos) : nodeType_(nodeType), pos_(std::move(pos)) {}

Node::~Node() = default;


const NodeType Node::getNodeType() const { return nodeType_; }

const FilePos Node::getFilePos() const { return pos_; }

std::ostream& operator<<(std::ostream& stream, const Node& node)
{
    node.print(stream);
    return stream;
}
