#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

using namespace llvm;

namespace{
    struct Insert_Function : public ModulePass{
    static char ID;
    Function * InsertFunc;
	Function * InsertFunc2;


    Insert_Function() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M)
    {


        errs() << "Entered Module " << M.getName() << "\n";

        for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
        {

			errs() << F->getName() << '\n';
            if(F->getName() == "MPU_Config")
            {
                errs() << "Find MPU Configure fucntion" << "\n";
                InsertFunc = &(*F);
                continue;
            }

            if(F->getName() == "MPU_zero_region")
            {
                errs() << "Find MPU zero region fucntion" << "\n";
                InsertFunc2 = &(*F);
                continue;
            }
 
		}

        for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
        {
            errs() << "Function name: " << F->getName() << "\n";

            if(F->getName() == "MPU_Config" || F->getName() == "MPU_zero_region"){
                continue;
            }
            if(F->getName() == "Read_Caller_size" || F->getName() == "Read_Caller_addr"){
                continue;
            }
            if(F->getName() == "read_execute" || F->getName() == "Config_execute"){
                continue;
            }

			for(Function::iterator BB = F->begin(), EE = F->end(); BB != EE; ++BB)
			{
				
				Function *movefn;
				BasicBlock *tmp_BB = &(*BB);

				LLVMContext& context = BB->getContext();

				Constant *start_addr;
				Constant *region_size;
				Constant *region_num;
						
				std::vector<Value *> arguments;

				if(tmp_BB == &(F->front()))
				{
					Instruction *first_inst = BB->getFirstNonPHI();
					Instruction *call_mpu;

					start_addr = ConstantInt::get(context, llvm::APInt(32,0x08004000,false));					
					region_size = ConstantInt::get(context, llvm::APInt(32,4,false));					
					region_num = ConstantInt::get(context, llvm::APInt(32,1,false));					

					arguments.clear();
					arguments.emplace_back(start_addr);
					arguments.emplace_back(region_size);
					arguments.emplace_back(region_num);
					
					call_mpu = CallInst::Create(InsertFunc,arguments,"");
					call_mpu->insertBefore(first_inst);

					/* MPU ZERO REGION */
/*
					if(F->getName() == "main")
					{
						start_addr = ConstantInt::get(context, llvm::APInt(32,0x08000000,false));					
						region_size = ConstantInt::get(context, llvm::APInt(32,13,false));					
						region_num = ConstantInt::get(context, llvm::APInt(32,0,false));					

						arguments.clear();
						arguments.emplace_back(start_addr);
						arguments.emplace_back(region_size);
						arguments.emplace_back(region_num);
					
						call_mpu = CallInst::Create(InsertFunc2,arguments,"");
						call_mpu->insertBefore(first_inst);
						
					}
					errs() << "Insert MPU_Config in First BB" << '\n';
*/
				}


				/* call instruction number is must less or equal than 1 */
				for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI)
				{
					Instruction *call_inst = &(*BI);

					if(BI->getOpcode() == 56)
					{
						auto *CB = (CallBase *)call_inst;
						movefn = CB->getCalledFunction();

						if(movefn->getName() != "MPU_Config" && movefn->getName() != "MPU_zero_region")
						{
							errs() << "find call : " << movefn->getName() << '\n';

							Instruction *call_mpu;
							
							start_addr = ConstantInt::get(context, llvm::APInt(32,0x08004002,false));					
							region_size= ConstantInt::get(context, llvm::APInt(32,0,false));					
							region_num = ConstantInt::get(context, llvm::APInt(32,2,false));					

							arguments.clear();
							arguments.emplace_back(start_addr);
							arguments.emplace_back(region_size);
							arguments.emplace_back(region_num);
				
							call_mpu = CallInst::Create(InsertFunc,arguments,"");
							call_mpu->insertBefore(call_inst);

							region_num = ConstantInt::get(context, llvm::APInt(32,1,false));					
							arguments.clear();
							arguments.emplace_back(start_addr);
							arguments.emplace_back(region_size);
							arguments.emplace_back(region_num);

							call_mpu = CallInst::Create(InsertFunc,arguments,"");
							call_mpu->insertBefore(call_inst->getNextNode());

							errs() << "Insert MPU_Config before call instruction" << '\n';
						}

					}
				}

						

                } /* iteration in Function */

        } /* iteraton in Module */
        return true;
    }	/* RunOnModule */
    }; /* Module Pass */
char Insert_Function::ID = 0;
static RegisterPass<Insert_Function> X("insert_function", "LLVM IR Insertion Function Pass");

}

