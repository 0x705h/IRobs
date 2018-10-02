
# IRobs

A very simple proof of concept for Out-Of-Tree _LLVM Pass_ binary target obfuscation.

## Dependencies

Download stable version of [LLVM](https://github.com/llvm-mirror/llvm/tree/stable)

	* git clone https://github.com/llvm-mirror/llvm.git
	* git checkout stable

Inside cloned LLVM directory, download [clang](https://github.com/llvm-mirror/clang)

	* git clone --depth=1 --branch=google/stable https://github.com/llvm-mirror/clang

Then compile LLVM. Building LLVM takes a lot of memory so configure your **cmake** accordingly! 
My building steps are the following:

	* cd [where_llvm_was_downloaded] && mkdir build && cd build 
	* cmake .. -DLLVM_USE_LINKER=gold -DCMAKE_BUILD_TYPE:STRING=Release -DLLVM_TARGETS_TO_BUILD:STRING="X86;ARM;AArch64" -DLLVM_INSTALL_UTILS=ON -DCMAKE_INSTALL_PREFIX=$HOME/llvm-stable
	* make -j [n-1] # which n is the number of processors 
	* be patient
	* make install 

LLVM should be installed at _$HOME/llvm-stable_

## Compile IRobs

	* mkdir build # at root of this project
	* cd build
	* cmake .. -DLLVM_ROOT=$HOME/llvm-stable
	* cd .. && make




