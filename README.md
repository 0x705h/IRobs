* download stable version of llvm https://github.com/llvm-mirror/llvm/tree/stable 
	* git clone https://github.com/llvm-mirror/llvm.git
	* git checkout stable

* inside llvm, download clang
	* git clone --depth=1 --branch=google/stable https://github.com/llvm-mirror/clang

* compile llvm, building LLVM takes a lot of memory, so configure your cmake accordingly! my config is the following:
	* cd [where_llvm_was_downloaded] && mkdir build && cd build && cmake .. -DLLVM_USE_LINKER=gold -DCMAKE_BUILD_TYPE:STRING=Release -DLLVM_TARGETS_TO_BUILD:STRING="X86;ARM;AArch64" -DLLVM_INSTALL_UTILS=ON -DCMAKE_INSTALL_PREFIX=$HOME/llvm-stable

	* make -j [n-1] # which n is the number of processors 
	* be patient
	* make install # llvm will be installed at $HOME/llvm-stable


Compile IROBS:
	* mkdir build # at root of this project
	* cd build
	* cmake .. -DLLVM_ROOT=$HOME/llvm-stable
	


