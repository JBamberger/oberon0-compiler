# Oberon-0-cpp
Oberon-0 compiler in C++. The produced assembly code performs arithmetic by using the processor as a stack machine
instead of using the registers efficiently.
e.g. ``c = a + b * c`` becomes something like:
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
function Invoke-Environment {
    param
    (
        # Any cmd shell command, normally a configuration batch file.
        [Parameter(Mandatory=$true)]
        [string] $Command
    )
    
    $Command = "`"" + $Command + "`""
    cmd /c "$Command > nul 2>&1 && set" | . { process {
        if ($_ -match '^([^=]+)=(.*)') {
            [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2])
        }
    }}
}

$vs = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
Invoke-Environment $vs

nasm -f win64 "Test.asm"
link /machine:x64 "Test.obj" msvcrt.lib legacy_stdio_definitions.lib
.\Test.exe
```