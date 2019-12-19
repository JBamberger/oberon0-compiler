#include "DeclarationsNode.h"
#include "NodeVisitor.h"
#include <utility>

template <class T, class D>
std::ostream& printList(std::ostream& stream,
                        const std::string& name,
                        const std::unique_ptr<std::vector<std::unique_ptr<T, D>>>& list_)
{
    stream << name << "(";
    if (!list_->empty()) {
        stream << *list_->at(0);
        for (size_t i = 1; i < list_->size(); ++i) {
            stream << ", " << *list_->at(i);
        }
    }
    stream << ")";
    return stream;
};

DeclarationsNode::DeclarationsNode(const FilePos& pos,
                                   std::unique_ptr<ConstDeclList> constants,
                                   std::unique_ptr<VarDeclList> vars,
                                   std::unique_ptr<TypeDeclList> types,
                                   std::unique_ptr<ProcDeclList> procedures)
    : Node(NodeType::declarations, pos), constants_(std::move(constants)),
      variables_(std::move(vars)), types_(std::move(types)), procedures_(std::move(procedures))
{
}

DeclarationsNode::~DeclarationsNode() = default;

const std::unique_ptr<ConstDeclList>& DeclarationsNode::getConstants() const
{
    return constants_;
}

const std::unique_ptr<VarDeclList>& DeclarationsNode::getVariables() const
{
    return variables_;
}

const std::unique_ptr<TypeDeclList>& DeclarationsNode::getTypes() const
{
    return types_;
}

const std::unique_ptr<ProcDeclList>& DeclarationsNode::getProcedures() const
{
    return procedures_;
}

void DeclarationsNode::visit(NodeVisitor* visitor) const { visitor->visit(this); }

void DeclarationsNode::print(std::ostream& stream) const
{
    stream << "Declarations(";
    printList(stream, "ConstantList", constants_) << ", ";
    printList(stream, "VariableList", variables_) << ", ";
    printList(stream, "TypeList", types_) << ", ";
    printList(stream, "ProcedureList", procedures_) << ")";
}
