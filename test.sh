#!/bin/sh
echo "Building normal target, main.native binary"
gcc -O0 main.c -o main.native
echo "Appliying IRobs passes"
~/llvm-stable/bin/clang -S -emit-llvm main.c -o main.ll
~/llvm-stable/bin/opt -S -load ./irobs/IRobs.so -IRobs main.ll > main.pass.ll
~/llvm-stable/bin/opt -S -load ./irobs/IRobs.so -IRobs main.pass.ll > main.pass2.ll
~/llvm-stable/bin/opt -S -load ./irobs/IRobs.so -IRobs main.pass2.ll > main.pass3.ll
~/llvm-stable/bin/opt -S -load ./irobs/IRobs.so -IRobs main.pass3.ll > main.pass4.ll
~/llvm-stable/bin/opt -S -load ./irobs/IRobs.so -IRobs main.pass4.ll > main.pass5.ll
~/llvm-stable/bin/opt -S -load ./irobs/IRobs.so -IRobs main.pass5.ll > main.pass6.ll
~/llvm-stable/bin/llc --x86-asm-syntax=intel -O0 main.pass6.ll -o main.s
echo "compiling main.native.pass binary..."
gcc -O0 main.s -o main.native.pass

