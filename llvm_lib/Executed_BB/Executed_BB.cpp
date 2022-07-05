#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"

#include <string>

using namespace llvm;

namespace{
    struct Executed_BB: public ModulePass{
    static char ID;

    Executed_BB() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M)
    {
		Function *read_check;
		Function *config_check;
        for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
		{	
			if(F->getName() == "read_execute")
			{
				read_check = &(*F);
				continue;
			}

			if(F->getName() == "Config_execute")
			{
				config_check = &(*F);
				continue;
			}
		}		

        for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
        {
            errs() << "Function name: " << F->getName() << "\n";
		
			if(F->getName() == "read_execute" || F->getName() == "Config_execute")
			{
				continue;
			}
			/*
			for(Function::iterator BB = F->begin(), EE = F->end(); BB != EE; ++BB)
			{
				BasicBlock *tmp_BB = &(*BB);
				LLVMContext& context = BB->getContext();
				Instruction *function_call;
				function_call = CallInst::Create(config_check,"");

				Instruction *inst = BB->getFirstNonPHI();
				function_call->insertBefore(inst);	

			}
			*/
			if(F->getName() == "MPU_Config")
			{
				for(Function::iterator BB = F->begin(), EE = F->end(); BB != EE; ++BB)
				{
					BasicBlock *tmp_BB = &(*BB);
					LLVMContext& context = BB->getContext();
					Instruction *function_call;
					function_call = CallInst::Create(config_check,"");

					Instruction *inst = BB->getFirstNonPHI();
					function_call->insertBefore(inst);	

					break;
				}
			}
			else if(F->getName() == "Read_Caller_addr" || F->getName() == "Read_Caller_size")
			{
				for(Function::iterator BB = F->begin(), EE = F->end(); BB != EE; ++BB)
				{
					BasicBlock *tmp_BB = &(*BB);
					LLVMContext& context = BB->getContext();
					Instruction *function_call;
					function_call = CallInst::Create(read_check,"");

					Instruction *inst = BB->getFirstNonPHI();
					function_call->insertBefore(inst);	
					
					break;
				}
			}

        }
        return true;
    }
    };
char Executed_BB::ID = 0;
static RegisterPass<Executed_BB> X("executed_bb", "Count Basic Block",
                                                            false   /*Only Looks at CFG*/,
                                                            false   /*Analysis Pass*/);

}

