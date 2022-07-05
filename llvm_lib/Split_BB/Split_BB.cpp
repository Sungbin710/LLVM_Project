#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
using namespace llvm;

namespace {
	struct Split_BB : public ModulePass {
		static char ID;
		Split_BB() : ModulePass(ID) {}

		virtual bool runOnModule(Module &M) {
		errs() << "Entered Module : " << M.getName() << "\n";

		for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
		{
			errs() << "Function Name : " << F->getName() << "\n";
			std::string F_name = (std::string)F->getName();
			/* exclude MPU related function */
			//if(F_name == "main" || F_name == "MPU_Config" || F_name == "MPU_zero_region" || F_name == "Read_Caller_addr" || F_name == "Read_Caller_size")
			if(F_name == "MPU_Config" || F_name == "MPU_zero_region" || F_name == "Read_Caller_addr" || F_name == "Read_Caller_size")
			{
				continue;
			}		


			for(Function::iterator BB = F->begin(), EE = F->end(); BB != EE;++BB)
			{
				//BasicBlock *tmpBB = &(*BB);

				int call_cnt = 0;
				for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI!= BE; ++BI)
				{

					Instruction *tmpInst = &(*BI);
					
					if( tmpInst->getOpcode() == 56 && (BB->getFirstNonPHI() != BB->getTerminator()) )  // call and not only one call instruction
					{
						call_cnt++;
						auto *CB = (CallBase *)tmpInst;
						//auto *CB_user = (User *)CB;
						Function *fn = CB->getCalledFunction();
						FunctionType *fnty = fn->getFunctionType();
						Type *ret_type = fnty->getReturnType();
					
					/*
						if((std::string)fn->getName() == "MPU_Config")
							continue;
					*/
						if(call_cnt > 1)
						{
							BB->splitBasicBlock(BI,"",true);
							errs() << "split BB!!\n";
						}
	
						/* if function type is void, has no store instruction behind call instruction */
						/*
						if(ret_type->isVoidTy())
						{
							// split can be done before specific instruction, so if instruction isn't exist begind call instruction -> error occur //
							if(BI->getNextNode() == BB->getTerminator())
							{
								break;
							}
							BB->splitBasicBlock(BI->getNextNode());
							errs() << "split BB!!\n";
							break;
						}
						
						if(BI->getNextNode()->getNextNode() == BB->getTerminator())
						{
							break;
						}
						BB->splitBasicBlock(BI->getNextNode()->getNextNode());
						errs() << "split BB!!\n";
						*/
					}
				}

			}
//}			
		}
		return true;
		}
	};
}
char Split_BB::ID = 0;
static RegisterPass<Split_BB> X("splitbb", "Split Basic Block", false, false);
