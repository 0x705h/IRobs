//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include <string>
#include <list>

using namespace llvm;

#define DEBUG_TYPE "hello"

STATISTIC(HelloCounter, "Counts number of functions greeted");

// Helper method for converting LLVM entities that support the print() method
// to a string.
// from https://github.com/eliben/llvm-clang-samples/blob/master/src_llvm/analyze_geps.cpp

template <typename T> static std::string ToString(const T *Obj) {
  std::string S;
  raw_string_ostream OS(S);
  Obj->print(OS);
  return OS.str();
}


namespace {
  struct IRobsPass : public FunctionPass {
  static char ID;
    public:
      IRobsPass() : FunctionPass(ID) {}

      virtual bool runOnFunction(Function &F) {
        // we will always modify every function
        // get current function name
        errs() << "IRobs current function: " << F.getName() << "\n";
        //const DataLayout &DL = BB.getModule()->getDataLayout();
        // Iterate all BasicBlocks on the current Function
        // and gather all BasicBlocks
        std::list<BasicBlock *> basicBlocksGathered;
        for ( Function::iterator IF = F.begin(), IF_e = F.end(); IF != IF_e; ++IF) {
            // Here we are iterating BasicBlocks of the function
            basicBlocksGathered.push_back(&*IF);
        }

        // Then for every BB let's put a new branch with some
        // gibberish

        while(! basicBlocksGathered.empty() ) {
          // get the first BB gathered
          BasicBlock * bb = basicBlocksGathered.front();
          createNewFlow(&F, bb);
          basicBlocksGathered.pop_front();
        }


        // return false to tell that function was not
        // modified by this pass, but now we'll modify
        // every function, so always return true
        return true;
      }

      void createNewFlow(Function *F, BasicBlock * BB) {

      }
  };
}


