#
# Copyright 2020 Jannik Bamberger
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

function Format-Cpp {
    param([switch]$All)
    # $files = Get-ChildItem -Recurse -Include *.h, *.cpp | ForEach-Object { $_.FullName }
    # clang-format.exe -i --style=file @files

    begin {
        if ($All) {
            $files = Get-ChildItem -Recurse -Include *.h, *.cpp | ForEach-Object { $_.FullName }
        }
        else {
            $files = @()
        }
    }
    process {
        if ($null -ne $_) {
            $files += , $_.FullName # append to the array
        }
    }
    end {
        Format-List -InputObject $files
        if ($files.Length -gt 0) {
            clang-format.exe -i --style=file @files
        }
        else {
            Write-Host "Provide some files names."
        }
    }
}

Get-ChildItem ast,codegen,scanner,util | Get-ChildItem -Recurse -include *.cpp,*.h | Format-Cpp