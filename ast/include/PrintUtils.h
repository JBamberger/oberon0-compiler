/**
 * Copyright 2020 Jannik Bamberger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <memory>
#include <string>
#include <vector>

template <class T, class D>
std::ostream& printList(std::ostream& stream,
                        const std::string& name,
                        const std::vector<std::unique_ptr<T, D>>& list_)
{
    stream << name << "(";
    if (!list_.empty()) {
        stream << *list_.at(0);
        for (size_t i = 1; i < list_.size(); ++i) {
            stream << ", " << *list_.at(i);
        }
    }
    stream << ")";
    return stream;
}
