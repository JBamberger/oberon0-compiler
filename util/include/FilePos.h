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

#include <iostream>
#include <string>

struct FilePos {
    std::string fileName;
    int lineNo, charNo;

    friend std::ostream& operator<<(std::ostream& os, const FilePos& obj)
    {
        if (!obj.fileName.empty()) {
            os << obj.fileName;
            if (obj.lineNo >= 0) {
                os << ":" << obj.lineNo;
                if (obj.charNo >= 0) {
                    os << ":" << obj.charNo;
                }
            }
        }
        return os;
    }
};
