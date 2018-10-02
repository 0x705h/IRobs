#!/bin/sh
echo "Building normal target, main.native binary"
~/llvm-stable/bin/clang -O0 main.c -o main.native
echo "Appliying IRobs passes"
~/llvm-stable/bin/clang -S -emit-llvm main.c -o main.ll
~/llvm-stable/bin/opt -S -load ./irobs/IRobs.so -IRobs main.ll
~/llvm-stable/bin/llc main.ll -o main.s
echo "compiling main.native.pass binary..."
gcc main.s -o main.native.pass

