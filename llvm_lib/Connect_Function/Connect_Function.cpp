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
    struct Connect_Function : public ModulePass{
    static char ID;
    Function *ReadFunc1;
    Function *ReadFunc2;
    Function *InsertFunc;

    Connect_Function() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M)
    {

        errs() << "Entered Module " << M.getName() << "\n";

        for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
        {
            if(F->getName() == "MPU_Config")
            {
                //errs() << "Find MPU_Config fucntion" << "\n";
                InsertFunc = &(*F);
                errs() << "Find: "<< InsertFunc->getName() << "\n";
                continue;
            }

            if(F->getName() == "Read_Caller_addr")
            {
                //errs() << "Find read_addr fucntion" << "\n";
                ReadFunc1 = &(*F);
                errs() << "Find: "<< ReadFunc1->getName() << "\n";
                continue;
            }

            if(F->getName() == "Read_Caller_size")
            {
                errs() << "Find read_size fucntion" << "\n";
                ReadFunc2 = &(*F);
                errs() << "Find: "<< ReadFunc2->getName() << "\n";
                continue;
            }
        }


        errs() << "==Find all fucntion==" << "\n";
        
		for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
        {

            errs() << "Function name: " << F->getName() << "\n";


            if(F->getName() == "Read_Caller_addr" || F->getName() == "Read_Caller_size"){
                continue;
            }
            if(F->getName() == "MPU_Config"){
                continue;
            }
            if(F->getName() == "MPU_zero_region"){
                continue;
            }
            if(F->getName() == "read_execute" || F->getName() == "Config_execute"){
                continue;
            }
            if(F->getName() == "main"){
                continue;
            }

			
		
			for(Function::iterator BB = F->begin(), EE = F->end(); BB != EE; ++BB)
			{
				/* first BB, first instruction */
				BasicBlock *tmp_BB = &(*BB);
				LLVMContext& context = BB->getContext();

				Value *addr;
				Value *size;
			
				
				/* first BB process */
				if(tmp_BB == &(F->front()) )
				{
////							
					//Instruction *A;
					//Instruction *B;

					Instruction *first_inst = BB->getFirstNonPHI();
					Instruction *Call1;
					Instruction *Call2;
					
					IRBuilder<> builder(first_inst);

					//A = builder.CreateAlloca(Type::getInt32Ty(context), nullptr, "read_addr");
					//B = builder.CreateAlloca(Type::getInt8Ty(context), nullptr, "read_size");
					//builder.CreateLoad(A, true, "nop");


					Call1 = CallInst::Create(ReadFunc1,"read_addr");
					Call2 = CallInst::Create(ReadFunc2,"read_size");
	

					Call1->insertBefore(first_inst);
					Call2->insertBefore(first_inst);

					//builder.CreateStore(Call1, A, true);
					//builder.CreateStore(Call2, B, true);
					

/*	
					auto *CB1 = (CallBase *)Call1;
					auto *CB2 = (CallBase *)Call2;
					for(auto arg = CB->arg_begin(); arg != CB->arg_end(); ++arg)
					{
						errs() << **arg << '\n';
					}	
*/					

						addr = Call1;
						size = Call2;
						errs() << " First BB  " << '\n';
				}
		
				/* last BB, last instruction */
				
				
				if( tmp_BB == &(F->back()) )
				{
				
					Instruction *last_inst = BB->getTerminator();

	
					std::vector<Value *> arguments;
					//Constant *start_addr = ConstantInt::get(context, llvm::APInt(32,0x20002001,false));
					Value *start_addr = addr; 
					Value *region_size = size;
					Value *region_num = ConstantInt::get(context, llvm::APInt(32,2,false));
				
					arguments.emplace_back(start_addr);
					arguments.emplace_back(region_size);
					arguments.emplace_back(region_num);
			
					Instruction *Call3;
					Call3 = CallInst::Create(InsertFunc, arguments,"");
					Call3->insertBefore(last_inst);
	
					errs() << " last BB " << '\n';
				}


			}
        } /* iteraton in Module */
        return true;
    }	/* RunOnModule */
    }; /* Module Pass */
char Connect_Function::ID = 0;
static RegisterPass<Connect_Function> X("connect_function", "Connect between caller and callee");

}

