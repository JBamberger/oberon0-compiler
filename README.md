<!--

  Copyright 2020 Jannik Bamberger

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

-->

# Oberon-0-cpp

Oberon-0 compiler in C++. The produced assembly code performs arithmetic by using the processor as a stack machine
instead of using the registers efficiently.
e.g. `c = a + b * c` becomes something like:

```nasm
mov  rax, @c     ; get c
push rax         ; stack: c
mov  rax, @b     ; get b
push rax         ; stack: b,c
pop  rax         ; rax := b
pop  rbx         ; rbx := c
imul rax, rbx    ; rax := b * c
push rax         ; stack: b*c
mov  rax, @a     ; get a
push rax         ; stack: a, b*c
pop  rax         ; rax := a
pop  rbx         ; rbx := b*c
add  rax, rbx    ; rax := a + b*c
push rax         ; stack: a+b*c
pop  rax         ; rax := a+b*c
mov  @c, rax     ; c := a+b*c
```

instead of 

```nasm
mov  rax, @c    ; rax := c
mov  rbx, @b    ; rbx := b
imul rax, rbx   ; rax := rax * rbx
mov  rbx, @a    ; rbx := a
add  rax, rbx   ; rax := a+b*c
mov  @c, rax    ; c := rax
```

Procedure calls are not implemented.

Bash:

```bash
./oberon0c Test.Mod Test.asm
nasm -felf64 Test.asm && gcc Test.o -o Test && ./Test
```

Cmd:

```cmd
C:\Program\ Files\ (x86)\Microsoft\ Visual\ Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat
nasm -f win64 "Test.asm"
link /machine:x64 "Test.obj" msvcrt.lib legacy_stdio_definitions.lib
.\Test.exe
```

Powershell:

```powershell
$vs = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
$vs = "`"" + $vs + "`""
cmd /c "$vs > nul 2>&1 && set" | . { process {
    if ($_ -match '^([^=]+)=(.*)') {
        [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2])
    }
}}

nasm -f win64 "Test.asm"
link /machine:x64 "Test.obj" msvcrt.lib legacy_stdio_definitions.lib
.\Test.exe
```
