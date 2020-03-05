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

Oberon-0 compiler written in C++. The compiler reads Oberon-0 code and produces assembly code for the
[Netwide Assembler (nasm)][nasm]. At the moment only arithmetic operations are implemented, no procedure calls. The
produced assembly performs arithmetic by using the processor as a stack machine instead of using the registers
efficiently.

## Example output

For an expression `c := a + b * c` as input the compiler produces assembly code similar to the following:

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

This is clearly not optimized. For example the lines `push rax` followed by `pop rax` are completely wasted and can be
omitted. Most of these sequences of `push` and `pop` could be eliminated easily by replacing them with a `mov` or
omission. In this example this would still not be optimal because all operations can take place within the registers
only. To achieve an even better result a register allocation algorithm would be necessary which could lead to code as
follows:

```nasm
mov  rax, @c    ; rax := c
mov  rbx, @b    ; rbx := b
imul rax, rbx   ; rax := rax * rbx
mov  rbx, @a    ; rbx := a
add  rax, rbx   ; rax := a+b*c
mov  @c, rax    ; c := rax
```

## Usage

First build the `oberon0c` target with cmake. This will produce a `oberon0c` executable.

To build an executable from your oberon0 program three steps are necessary:

- Compilation with `oberon0c`: oberon0 -> assembly
- Assembly with `nasm`: assembly -> object
- Linking with `link` or `gcc`: object -> executable

To compile a file invoke the compiler with the input file as first argument and output file as second arg. Example:
`./oberon0c Test.Mod Test.asm`.

### Linux & gcc

This section assumes that gcc is available. On Linux we want to create an executable using the `elf` object format for a
64bit machine.

```bash
nasm -felf64 Test.asm && gcc Test.o -o Test && ./Test
```

### Windows

This section assumes that Visual Studio 2019 is installed, such that its linker can be used. To make the linker
available VS provides a script called `vcvars64.bat`. This will set up some variables, including the linker and required
locations of binaries. Therefore this must be executed first before the linker can be run.

On windows we must tell `nasm` to output a `win64` object file.

#### Cmd

```cmd
C:\Program\ Files\ (x86)\Microsoft\ Visual\ Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat
nasm -f win64 "Test.asm"
link /machine:x64 "Test.obj" msvcrt.lib legacy_stdio_definitions.lib
.\Test.exe
```

#### Powershell

Running in a Powershell environment is a bit tricky because the `vcvars64` script does not work here. It can be invoked
but the variables set in the script will not be visible in Powershell. Therefore the script is executed in a command
 window. Then set is called to print all environment variables. This output is piped into a Powershell block that sets
 all of the variables in the Powershell environment.

```powershell
$vs = "`"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat`""
cmd /c "$vs > nul 2>&1 && set" | . { process {
    if ($_ -match '^([^=]+)=(.*)') {
        [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2])
    }
}}

nasm -f win64 "Test.asm"
link /machine:x64 "Test.obj" msvcrt.lib legacy_stdio_definitions.lib
.\Test.exe
```

[nasm]: https://nasm.us/