cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_FLAGS "-std=c++11")
# LLVM detection part
# The user is supposed to set this to a valid llvm 3.7 install root
set(LLVM_ROOT "" CACHE PATH "Root of LLVM install.")

set(LLVM_DIR "${LLVM_ROOT}/lib/cmake")

# A bit of a sanity check:
if(NOT EXISTS ${LLVM_ROOT}/include/llvm )
    message(FATAL_ERROR
            "LLVM_ROOT (${LLVM_ROOT}) is invalid")
endif()

if(NOT EXISTS ${LLVM_ROOT}/bin/clang )
    message(FATAL_ERROR
        "clang not compiled with LLVM")
endif()

# Load various LLVM config stuff,
# see http://llvm.org/docs/CMake.html#developing-llvm-passes-out-of-source
#list(APPEND CMAKE_PREFIX_PATH
#     "${LLVM_ROOT}/share/llvm/cmake")
set(CMAKE_PREFIX_PATH
	"${LLVM_ROOT}/lib/cmake/llvm")
set(LLVM_CMAKE_DIR "${LLVM_ROOT}/lib/cmake/llvm")
find_package(LLVM REQUIRED CONFIG)

#list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
set(CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
#include(LLVM-Config)
#include(HandleLLVMOptions) # load additional config
include(AddLLVM) # used to add our own modules

# propagate LLVM-specific variables to this project
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

# See commit r197394, needed by add_llvm_module in llvm/CMakeLists.txt *FIXED in 3.8*
#set(LLVM_RUNTIME_OUTPUT_INTDIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INT_DIR}")
#set(LLVM_LIBRARY_OUTPUT_INTDIR "${CMAKE_BINARY_DIR}/lib/${CMAKE_CFG_INT_DIR}")

# Following commands are specific to this tutorial

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(irobs)


