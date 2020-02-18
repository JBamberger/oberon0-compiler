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

$vs = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
$vs = "`"" + $vs + "`""
cmd /c "$vs > nul 2>&1 && set" | . { process {
    if ($_ -match '^([^=]+)=(.*)') {
        [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2])
    }
}}

$name = "Test.asm"

$file = Get-Item $name;
if ($file.Extension -ne ".asm") {
    Write-Error "Not an asm file."
    return;
}

nasm -f win64 $file.FullName
if (!$?) {
    Write-Error "Could not assemble."
    return;
}

$obj = Get-Item "$($file.BaseName).obj"
if (!$obj) {
    Write-Error "Could not assemble."
    return;
}

link /machine:x64 $obj msvcrt.lib legacy_stdio_definitions.lib

if (!$?) {
    Write-Error "Could not link."
    return;
}

.\Test.exe