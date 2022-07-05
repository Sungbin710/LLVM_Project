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
#include <iostream>
#include <fstream>
#include <string>

using namespace llvm;

/* class to include bb information */
class func{
public:
    std::string name;
	uint32_t function_size = 0;
	uint32_t region_start = 0x00;
	uint32_t region_size = 0;

    void print()
    {
        std::cout << "function name: " << name << std::endl;
        std::cout << "region start: " << region_start << std::endl;
        std::cout << "region size: " << region_size << std::endl;

    }
};

uint32_t to_size(uint32_t size);

namespace{
    struct Set_Operand: public ModulePass{
    static char ID;
    Set_Operand() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M)
    {

/* read file and construct class */

		int Funcs_size;
        std::vector<func *> Funcs;
        std::ifstream input_file;
        std::string line;

		// read function name / BB size //
        input_file.open("/home/sblee/llvm-project/build/basic_block/information/Link_addr.txt");
        std::string delimiter = ":";
        func *f;
        int cnt = 0;
		int i = 0;
        while(getline(input_file, line))
        {	
            if(line.size() > 1)
            {
                int pos = line.find(delimiter);
                std::string token = line.substr(pos+2);
				std::cout << token << std::endl;

				if(cnt == 0)
				{
					f = new func();
					Funcs.emplace_back(f);
					f->name = token;
				}
				else if(cnt == 1)
				{
					Funcs[i]->function_size = stoi(token);
				}
				else if(cnt == 2)
				{
					Funcs[i]->region_start = stoi(token);
				}
				else if(cnt == 3)
				{
					Funcs[i]->region_size = stoi(token);
				}
				cnt++;

				if(cnt == 4)
					cnt = 0;
            }
			else
			{
				i++;
			}
        };
		input_file.close();

		/* for debugging */
		Funcs_size = Funcs.size();
        for(int i=0; i<Funcs_size; i++)
        {
            Funcs[i]->print();
        }


/*************************************************************************/

    	    for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
    	    {
    		    errs() << "Function name: " << F->getName() << "\n";

    		    for(Function::iterator BB = F->begin(), EE = F->end(); BB != EE; ++BB)
    		    {

					Function *calledfn;
					std::string fname;
					
					BasicBlock *tmp_BB = &(*BB);
					fname = (std::string)F->getName();

    			    for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI)
		    	    {
						Instruction *inst = &(*BI);
	    			    LLVMContext& context = BI->getContext();

						if(BI->getOpcode() == 56)
						{
							auto *CB = (CallBase *)inst;
							auto *CB_User = (User *)CB;
							calledfn = CB->getCalledFunction();
							fname = (std::string)calledfn->getName();

/* call region */
							if(fname == "MPU_Config")
							{

								std::vector<Constant *> arguments;
								arguments.clear();

								ConstantInt *start_addr;
								ConstantInt *re_size;
								ConstantInt *re_num;
/* own region */
								ConstantInt *tmp_num =  (ConstantInt *)CB_User->getOperand(2); 
								APInt t_num = tmp_num->getValue();
								errs() << "t_num: " << t_num << '\n';

								ConstantInt *set_num;
								uint32_t region_num = 0;
								set_num = ConstantInt::get(context,llvm::APInt(32,1,false));
								APInt s_num = set_num->getValue();
								errs() << "s_num: " << s_num << '\n';
								
								if(t_num == 1) /* function start / after call(ret) */
								{
									int i;
									for(i=0; i<Funcs_size; i++)
									{
										if(F->getName() == Funcs[i]->name)
											break;
											
									}
									
									start_addr = ConstantInt::get(context, llvm::APInt(32, Funcs[i]->region_start, false));
									re_size = ConstantInt::get(context, llvm::APInt(32, to_size(Funcs[i]->region_size), false));


									CB_User->setOperand(0, start_addr);
									CB_User->setOperand(1, re_size);
								}
								else if(t_num == 2) /* before call */
								{
									errs() << " region 2 process " << '\n';

									if(BI->getNextNode()->getOpcode() == 56)
									{
										Instruction *nextinst = BI->getNextNode();
										auto *NCB = (CallBase *)nextinst;
										Function *Ncalledfn = NCB->getCalledFunction();
										std::string Nfname = (std::string)Ncalledfn->getName();
									
										int i;
										for(i=0; i<Funcs_size; i++)
										{
											if(Nfname  == Funcs[i]->name)
												break;
											
										}
	
										start_addr = ConstantInt::get(context, llvm::APInt(32, Funcs[i]->region_start, false));
										re_size = ConstantInt::get(context, llvm::APInt(32, to_size(Funcs[i]->region_size), false));

										CB_User->setOperand(0, start_addr);
										CB_User->setOperand(1, re_size);

								}

								}

							} /* fname == "MPU_Config" */
							
						}

				
			 	  	} /*basic block iterator */
	    		} /*function iterator */
       	   	} /* module iterator */
			return true;
   	} 
    };
char Set_Operand::ID = 0;
static RegisterPass<Set_Operand> X("set_operand", "LLVM chage Function call arguments");

}

uint32_t to_size(uint32_t size)
{
	uint32_t region = 0b00100;
	uint32_t mini = 32;

	if((size >= 32 && size <= 0x100000000) && (size % 32 == 0))
	{
		while(mini < size)
		{
			mini *= 2;
			region += 1;
		}
	}
	else
	{
		std::cout << "error -> to make size 32, 64 ... 512 " << std::endl;
	}

	return region;
}
