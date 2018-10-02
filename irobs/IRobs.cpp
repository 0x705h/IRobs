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
  struct IRobsPass : public BasicBlockPass {
  static char ID;
    public:
      IRobsPass() : BasicBlockPass(ID) {}

      virtual bool runOnBasicBlock(BasicBlock &BB) {
        const DataLayout &DL = BB.getModule()->getDataLayout();
        // Iterate all instructions on the current BB
        for ( BasicBlock::iterator II = BB.begin(), II_e = BB.end(); II != II_e; ++II) {
            
            
            if(BranchInst *bInst = dyn_cast<BranchInst>(II)) {
              bool isConditional = bInst->isConditional();
              errs() << "I am a BranchInstruction and I am conditional\n";
            }

        }

        // return false to tell that basic block was not
        // modified by this pass
        return false;
      }
  };



} // end of anon namespace

char IRobsPass::ID = 0;
static RegisterPass<IRobsPass> X("IRobs", "Mess around with BB", false, false);
/*

namespace {
  // Hello - The first implementation, without getAnalysisUsage.
  struct Hello : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    Hello() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      ++HelloCounter;
      errs() << "Hello: ";
      errs().write_escaped(F.getName()) << '\n';
      return false;
    }
  };
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass");

namespace {
  // Hello2 - The second implementation with getAnalysisUsage implemented.
  struct Hello2 : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    Hello2() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      ++HelloCounter;
      errs() << "Hello: ";
      errs().write_escaped(F.getName()) << '\n';
      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }
  };
}

char Hello2::ID = 0;
static RegisterPass<Hello2>
Y("hello2", "Hello World Pass (with getAnalysisUsage implemented)");
*/