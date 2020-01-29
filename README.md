# Oberon-0-cpp
Oberon-0 compiler in C++

```bash
./oberon0c Test.Mod Test.asm
nasm -felf64 Test.asm && gcc Test.o && ./a.out
```