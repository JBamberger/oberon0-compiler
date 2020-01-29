#!/bin/sh

nasm -f elf64 Test.asm && gcc Test.o -o Test && ./Test

