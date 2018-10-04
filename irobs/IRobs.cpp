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
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/TypeBuilder.h"
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
        LLVMContext &ctx = F->getContext();
        IRBuilder<> builder(ctx);
        // creating a condition
        Value * LHS = ConstantInt::get(Type::getInt32Ty(F->getContext()), 0x41414141, false );
        Value * RHS = ConstantInt::get(Type::getInt32Ty(F->getContext()), 0x42424242, false );

        // The always false condition. End of the first block
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
              // Now that we have the TerminatorInst of the split operation
              // WHICH POINTS TO THE NEWLY CREATED BB on the split!!!
              // lets add some instructions to the new BB

              // on creating new instructions
              // http://llvm.org/docs/ProgrammersManual.html#creating-and-inserting-new-instructions
              //Value * int32PtrTy = Type::getInt32PtrTy
              //Value * inmediate = ConstantInt::get(Type::getInt32Ty(F->getContext()), 0x424f475553, false);
              Type  * ty = Type::getInt32Ty(ctx);
              Value *iptr = ConstantInt::get(PointerType::getInt32Ty(F->getContext()), 0x424f475553, false);
              
              Twine * variable_str = new Twine("variable_str");
              Twine * bogus = new Twine("bogus_str");


              Twine * rogue_var1_str = new Twine("rogue_var1");
              Twine * rogue_var2_str = new Twine("rogue_var2");
              Value * rval1 =ConstantInt::get(Type::getInt32Ty(ctx), 0, false);
              Value * rval2 =ConstantInt::get(Type::getInt32Ty(ctx), 0, false);

              GlobalVariable 	* r1 = new GlobalVariable(
                *F->getParent(), 
                Type::getInt32Ty(ctx), false,
                GlobalValue::CommonLinkage, 
                (Constant * )rval1,
                *rogue_var1_str);
              GlobalVariable 	* r2 = new GlobalVariable(
                *F->getParent(), 
                Type::getInt32Ty(ctx), false,
                GlobalValue::CommonLinkage, 
                (Constant * )rval2,
                *rogue_var2_str);

              LoadInst * load1, * load2;
              StoreInst * store1, * store2;
              load1 = new LoadInst ((Value *)r1, "new_load1", ti);
              load2 = new LoadInst ((Value *)r2, "new_load2", ti);
              
              Twine * alloca_str = new Twine("alloca_str");
              
              store1 = new StoreInst (rval2, r1, ti);
              store1 = new StoreInst (rval1, r2, ti);

              // update the iterators
              IB = BB->begin();
              IB_e = BB->end();

              errs() << "Finished Splitting, countInst: " << countInst << " ...\n";
            }

            countInst++;
            
        }


      }
  };
}

char IRobsPass::ID = 0;
static RegisterPass<IRobsPass> X("IRobs", "Mess around with BB", false, false);
