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
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/Constants.h"

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
        errs() << "BB Total:" << basicBlocksGathered.size() << "\n";
        // Then for every BB let's put a new branch with some
        // gibberish

        while(! basicBlocksGathered.empty() ) {
          // get the first BB gathered
          BasicBlock * bb = basicBlocksGathered.front();
          createNewFlow(&F, bb);
          basicBlocksGathered.pop_front();
          errs() << "BB Left:" << basicBlocksGathered.size() << "\n";
        }


        // return false to tell that function was not
        // modified by this pass, but now we'll modify
        // every function, so always return true
        return true;
      }

      void createNewFlow(Function *F, BasicBlock * BB) {
        // http://llvm.org/doxygen/classllvm_1_1BasicBlock.html#a19445f836d9e1ecb32cba27ec4338fff
        // ok, docs recomends to use BasicBlockUtils

        // creating a condition
        Value * LHS = ConstantInt::get(Type::getInt64Ty(F->getContext()), 0x41414141, false );
        Value * RHS = ConstantInt::get(Type::getInt64Ty(F->getContext()), 0x41414141, false );

        // The always true condition. End of the first block
        Twine * deadbeef = new Twine("deadbeefalwaystrue");
        
        Instruction * splitBefore;
        
        int countInst = 0;
        // Iterate the instructions of the BB
        for ( BasicBlock::iterator IB = BB->begin(), IB_e = BB->end(); IB != IB_e; ++IB) {
            // if the instruction is the fourth of the basic block
            splitBefore = &*IB;  
            if(countInst == 4) {
              // This instruction compares its operands according to the predicate given to the constructor
              // http://llvm.org/doxygen/classllvm_1_1FCmpInst.html
              ICmpInst * cond = new ICmpInst(splitBefore, ICmpInst::ICMP_EQ , LHS, RHS, *deadbeef);

              // then split the BB here
              errs() << "Splitting...\n";
              TerminatorInst * ti = llvm::SplitBlockAndInsertIfThen(cond, splitBefore, false, nullptr, nullptr);

              /*   
              At this point we generated a split
              %deadbeefalwaystrue = icmp eq i64 1094795585, 1094795585
              br i1 %deadbeefalwaystrue, label %2, label %3

              ; <label>:2:                                      ; preds = %entry
                br label %3

              ; <label>:3:                                      ; preds = %entry, %2
                %cmp = icmp eq i32 %add, 0
                br i1 %cmp, label %if.then, label %if.end

            */

              errs() << "Finished Splitting, countInst: " << countInst << " ...\n";
              return;
            }
            errs() << "Outside if, countInst: " << countInst << " \n";
            IB++;
            countInst++;
            
        }


      }
  };
}

char IRobsPass::ID = 0;
static RegisterPass<IRobsPass> X("IRobs", "Mess around with BB", false, false);
