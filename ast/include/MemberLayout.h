#pragma once

#include "FieldDeclarationNode.h"
#include "Node.h"
#include "Scope.h"
#include "TypeNode.h"
#include <algorithm>
#include <vector>

template <class T>
struct Member {
    std::unique_ptr<T> member;
    size_t offset;
};

template <class T>
class MemberLayout {
    const size_t stack_width = 16;
    size_t prev_bound = 0;
    std::vector<Member<T>> members_;

  public:
    void insert(std::unique_ptr<T> member)
    {
        const auto size = member->getType()->getByteSize();
        const auto prev_offset = members_.empty() ? 0 : members_.at(members_.size() - 1).offset;
        auto next_offset = prev_offset + size;

        if (next_offset > prev_bound + stack_width) {
            // The stack is not aligned.
            // Assumes that data types are well behaved and align the internals correctly.
            prev_bound += stack_width;       // move to the next stack alignment boundary
            next_offset = prev_bound + size; // fix the offset value
            // depending on the size increment the location of the boundary
            prev_bound += (size / stack_width) * stack_width;
        }

        members_.push_back({std::move(member), next_offset});
    }

    Member<T>& at(size_t pos) const { return members_.at(pos); }

    Member<T>& at(const std::string& name)
    {
        for (auto& m : members_) {
            if (m.member->getName() == name) {
                return m;
            }
        }
        throw std::runtime_error("Invalid member name '" + name + "'.");
    }

    const Member<T>& at(const std::string& name) const
    {
        for (const auto& m : members_) {
            if (m.member->getName() == name) {
                return m;
            }
        }
        throw std::runtime_error("Invalid member name '" + name + "'.");
    }

    size_t getSize() const
    {
        // the last offset is also the size of the structure
        // no alignment is necessary here, because the members are aligned correctly if the starting
        // position is valid.
        if (members_.empty()) {
            return 0;
        }
        const auto last = members_.at(members_.size() - 1).offset;
        if (prev_bound == last) {
            return last;
        }
        return prev_bound + stack_width;
    }

    template <typename Functor>
    void visitMembers(Functor f)
    {
        std::for_each(members_.begin(), members_.end(), f);
    }

    template <typename Functor>
    void visitMembers(Functor f) const
    {
        std::for_each(members_.begin(), members_.end(), f);
    }
};
