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
#include <string>
#include <iostream>
#include <fstream>

using namespace llvm;

namespace{
    struct Find_Function: public ModulePass{
    static char ID;

	std::vector<std::string> Funcs;
	
    Find_Function() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M)
    {

        for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
        {
            errs() << "Function name: " << F->getName() << "\n";

            std::string F_name = (std::string)(F->getName());
			/* exclude MPU related function */
			if(F_name != "MPU_Config" && F_name != "MPU_zero_region" && F_name != "Read_Caller_addr" && F_name != "Read_Caller_size" && F_name != "Config_execute" && F_name != "read_execute")
				Funcs.emplace_back(F_name);

            for(Function::iterator BB = F->begin(), EE = F->end(); BB != EE; ++BB)
            {

            }
        }

		std::string path = "/home/sblee/llvm-project/build/basic_block/information/";
		std::string func_file = path + "function.txt";
		std::ofstream f;
		int len = Funcs.size();
		f.open(func_file);
		for(int i=0; i<len; i++)
		{
			f << Funcs[i] << std::endl;
		}
		f.close();	
		

        return true;
    }
    };
char Find_Function::ID = 0;
static RegisterPass<Find_Function> X("find_function", "Find every Function",
                                                            false   /*Only Looks at CFG*/,
                                                            false   /*Analysis Pass*/);

}

