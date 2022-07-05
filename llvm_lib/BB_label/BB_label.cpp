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
    struct BB_label : public ModulePass{
    static char ID;

    BB_label() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M)
    {

        for(Module::iterator F = M.begin(), E = M.end(); F != E; ++F)
        {
            errs() << "Function name: " << F->getName() << "\n";

            int Basic_Block_cnt = 0;
            int Basic_Block_size = 0;

            std::string F_name = (std::string)(F->getName());

            for(Function::iterator BB = F->begin(), EE = F->end(); BB != EE; ++BB)
            {

                Basic_Block_cnt++;
                Basic_Block_size = BB->sizeWithoutDebug();

                std::string num = std::to_string(Basic_Block_cnt);
				std::string BB_name = F_name + "_BB" + num;
                BB->setName(BB_name);

                errs() << "Basic Block [" << Basic_Block_cnt << "]: " << BB->getName();
                //BB->printAsOperand(errs(), false);
                errs() << ", num of instructions [" << Basic_Block_size << "]\n";

/* find branch instruction

                for(BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI)
                {
                        int operands = 0;
                        operands = BI->getNumOperands();        // get operand num

                        if(BI->getOpcode() == 56)       // call
                        {
                                errs() << "command: " <<  BI->getOpcodeName() << "(" << BI->getOpcode() << ")\n";
                                for(int i=0;i<operands;i++)
                                {
                                        //errs() << "   operand" <<  i << " : " << BI->getOperand(i)->getValueName()  << '\n';
                                        errs() << "     operand" <<  i << " : " << BI->getOperand(i)->getNameOrAsOperand()  << '\n';
                                }

                        }
                        else if(BI->getOpcode() == 2)   // br
                        {

                                errs() << "command: " <<  BI->getOpcodeName() << "(" << BI->getOpcode() << ")\n";
                                for(int i=0;i<operands;i++)
                                {
                                        //errs() << "   operand" <<  i << " : " << BI->getOperand(i)->getName()  << '\n';
                                        errs() << "     operand" <<  i << " : " << BI->getOperand(i)->getNameOrAsOperand()  << '\n';
                                }

                        }
                        else if(BI->getOpcode() == 1)   // ret
                        {

                                errs() << "command: " <<  BI->getOpcodeName() << "(" << BI->getOpcode() << ")\n";

                                for(int i=0;i<operands;i++)
                                {
                                        errs() << "     operand" <<  i << " : " << BI->getOperand(i)->getNameOrAsOperand()  << '\n';
                                }


                        }
                        else
                        {
                                errs() << "command: " <<  BI->getOpcodeName() << "(" << BI->getOpcode() << ")\n";
                        }
                }
*/
            }
        }
        return true;
    }
    };
char BB_label::ID = 0;
static RegisterPass<BB_label> X("bb_label", "Anaylsis Basic Block",
                                                            false   /*Only Looks at CFG*/,
                                                            false   /*Analysis Pass*/);

}

