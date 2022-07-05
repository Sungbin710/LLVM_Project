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

/* function to insert instruction num */
int need_size(int BB_size);
void fill_nop(IRBuilder<> &builder, Instruction *A, Constant *zero, int need_size);

/* class to include bb information */
class func{
public:
    std::string name;
    int BB_count = 0;
    std::vector<int> BB_size;
    std::vector<int> BB_need_size;

    void print()
    {
        std::cout << "function name: " << name << std::endl;
        for(int i=0; i<BB_count; i++)
        {
            std:: cout << "BB" << i+1 << " size: " << BB_size[i] << std::endl;
            std:: cout << "BB" << i+1 << " need size: " << BB_need_size[i] << std::endl;
        }

    }
};


namespace{
    struct Insert_Nop : public ModulePass{
    static char ID;

    Insert_Nop() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M)
    {


/*********************************************************************/
/* read file and construct class */

	    std::vector<func *> Funcs;
	    std::ifstream input_file;
	    std::string line;
	    
	    input_file.open("/home/sblee/llvm-project/build/basic_block/information/middle_BB.txt");
	    std::string delimiter = ":";
	    func *f;
	    int f_cnt = 0;
	    int Basic_block_size = 0;

	    while(getline(input_file, line))
	    {
		    if(line.size() > 1)
		    {
	    	
			    int pos = line.find(delimiter);
			    std::string token = line.substr(pos+2);
			    int cnt = Funcs.size()-1;
			  	

			    if(!isdigit(token[0]))
			    {
				    f = new func();
				    Funcs.emplace_back(f);
				    f->name = token;
				    std::cout << f->name << std::endl;
			    }
			    else
			    {
				    Basic_block_size = stoi(token);

				    Funcs[cnt]->BB_size.emplace_back(Basic_block_size);
				    std::cout << Funcs[cnt]->BB_size[Funcs[cnt]->BB_count] << std::endl;
				    //Funcs[cnt]->BB_need_size.emplace_back(need_size(Basic_block_size));
				    Funcs[cnt]->BB_need_size.emplace_back(128-Basic_block_size);
		    	    Funcs[cnt]->BB_count += 1;
			    }
		    }
	    };
		int Funcs_size = Funcs.size();
			
	    for(int i=0; i<Funcs_size; i++)
	    {
		    Funcs[i]->print();
	    }
		    
	    input_file.close();


		Funcs[0]->BB_need_size[0] += 2;
		Funcs[1]->BB_need_size[0] -= 4;
		Funcs[2]->BB_need_size[0] -= 4;
		Funcs[3]->BB_need_size[0] += 2;
		Funcs[5]->BB_need_size[0] -= 4;
		Funcs[6]->BB_need_size[0] -= 4;
	/* expint 4, 12 */
	/*
		Funcs[2]->BB_need_size[4] -= 2;
		Funcs[2]->BB_need_size[12] -= 2;

		Funcs[2]->BB_need_size[2] -= 2;
		Funcs[3]->BB_need_size[0] -= 4;
		Funcs[4]->BB_need_size[0] -= 4;
	*/
	/* fac */
	/*
		Funcs[1]->BB_need_size[0] -= 2;
		Funcs[2]->BB_need_size[0] -= 4;
		Funcs[3]->BB_need_size[1] -= 2;
	*/
	/* fibcall */
	/*	
		Funcs[1]->BB_need_size[3] -= 4;
		Funcs[2]->BB_need_size[0] -= 4;
	*/	
	/* janne_complex */
	/*
		Funcs[1]->BB_need_size[7] -= 2;
		Funcs[3]->BB_need_size[0] -= 4;
	*/
	/* select */
	/*
		Funcs[1]->BB_need_size[9] -= 2;
		Funcs[1]->BB_need_size[46] -= 2;

		Funcs[1]->BB_need_size[3] -= 2;
		Funcs[1]->BB_need_size[4] -= 2;
		Funcs[1]->BB_need_size[13] -= 2;
		Funcs[1]->BB_need_size[17] -= 2;
		Funcs[1]->BB_need_size[21] -= 2;
		Funcs[1]->BB_need_size[28] -= 2;
		Funcs[1]->BB_need_size[31] -= 2;
		Funcs[1]->BB_need_size[35] -= 2;
		Funcs[2]->BB_need_size[0] -= 4;
		Funcs[3]->BB_need_size[0] -= 4;
	*/

/* libud */

//		Funcs[0]->BB_need_size[0] += 2;
/*
		Funcs[2]->BB_need_size[0] -= 4;
		Funcs[4]->BB_need_size[4] -= 2;
		Funcs[4]->BB_need_size[6] -= 2;
		Funcs[4]->BB_need_size[15] -= 2;
		Funcs[4]->BB_need_size[25] -= 2;
		Funcs[4]->BB_need_size[33] -= 2;
*/
/*************************************************************************/


	    for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
    	    {
		    errs() << "Function name: " << F->getName() << "\n";

		    for(Function::iterator BB = F->begin(), EE = F->end(); BB != EE; ++BB)
		    {
			    Instruction *A;
			    //BasicBlock *Block = &(*BB);



		    	for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI)
			    {
           
				    //Instruction *inst = &(*BI);
				    Instruction *inst = BB->getFirstNonPHI();
				    LLVMContext& context = BI->getContext();
		    	    Constant *zero = ConstantInt::get(context, llvm::APInt(32,0,false));
				
					//Value *L = ConstantInt::get(Type::getInt8Ty(context),1);
					//Value *R = ConstantInt::get(Type::getInt8Ty(context),0);

				    IRBuilder<> builder(inst);
					
				    std::string BB_name = (std::string)BB->getName(); 
				    int len = BB_name.size();
				    int num_index=0;
				
				    while(std::isdigit(BB_name[len-num_index-1]))
				    {
					    num_index++;
				    }
				    std::string seq = BB_name.substr(len-num_index,len);
				    int num = stoi(seq);
/*
					LoadInst *LA;
					LoadInst *LB;
					Value *var1;
					Value *var2;
					var1 = ConstantInt::get(Type::getInt32Ty(context),0);
					var2 = ConstantInt::get(Type::getInt32Ty(context),0);
*/
/*
					if(num == 1)
					{
						A = builder.CreateAlloca(Type::getInt16Ty(context), nullptr, "a");
						builder.CreateStore(var1, A, true);
					}
*/					
					//A = BB->getFirstNonPHIOrDbg();
					//assert(A);
					//builder.CreateLoad(A, true, "nop");
					//var1 = BinaryOperator::Create(Instruction::Add, var1, var1, "nop", LA);
					//var1 = BinaryOperator::Create(Instruction::Add, var1, var1, "nop", LA);
					//builder.CreateStore(var1, A, true);
					//var1 = BinaryOperator::Create(Instruction::Add, var1, var1, "nop", A);
					//var1 = builder.CreateAdd(builder.getInt8(0),LA);
//					break;
					

					if(num == 1)
					{
						A = builder.CreateAlloca(Type::getInt32Ty(context), nullptr, "nop");
						    //builder.CreateStore(zero, A, true);
					}

					for(int i=0;i<Funcs_size;i++)
					{
						
					 	    if(Funcs[i]->name == (std::string)F->getName())
						    {
								fill_nop(builder, A, zero, Funcs[i]->BB_need_size[num-1]-2);
								errs() << "fill_nop in BB!\n";
								break;
						    }
						
					}
				
					break;
					
					

//					    builder.CreateLoad(A, /*isVolatile=*/ true, "nop");
//					    builder.CreateStore(zero, A, /*isVolatile=*/ true);

					    /*
					     *	auto *A = builder.CreateAlloca(Type::getInt32Ty(context), nullptr, "nop");
					     *  builder.CreateStore(zero, A, true);
					     *  auto *B = builder.CreateLoad(A, true, "nop");
					     *  auto *C = builder.CreateAdd(B, zero, "addtmp");
					     *  builder.CreateStore(C, B, true);
					     */


			    }	/* BB iterator */
	    	    }	/* Function iterator */
	    }	/* Module iterator */
        return true;
    }
    };
char Insert_Nop::ID = 0;
static RegisterPass<Insert_Nop> X("insert_nop", "LLVM IR Insertion No operation Pass");

}

int need_size(int BB_size)
{
    int region_size = 32;
    while(BB_size > region_size)
    {
        region_size *= 2;
    }
    return region_size - BB_size;
}

void fill_nop(IRBuilder<> &builder, Instruction *A, Constant *zero, int need_size)
{
	while(need_size > 0){
		builder.CreateStore(zero, A, /*isVolatile=*/ true);
		//builder.CreateLoad(A,true, "nop");
		need_size -= 2;	
	}
}

