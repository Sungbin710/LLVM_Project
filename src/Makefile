CC = clang

OPT_COMMAND = /home/sblee/llvm-project/build/bin/opt
OFLAGS = -enable-new-pm=0

LIB1 = /home/sblee/llvm-project/build/lib/Find_Function.so
pass1 = -find_function
LIB2 = /home/sblee/llvm-project/build/lib/BB_label.so
pass2 = -bb_label
#############################################################
LIB3 = /home/sblee/llvm-project/build/lib/Insert_Function.so
pass3 = -insert_function
LIB4 = /home/sblee/llvm-project/build/lib/Connect_Function.so
pass4 = -connect_function
LIB5 = /home/sblee/llvm-project/build/lib/Set_Operand.so
pass5 = -set_operand

MACH= cortex-m4
TRIPLE= --target=thumbv7em-unknown-none-eabi
FLAGS= -mcpu=$(MACH) -mfloat-abi=soft -emit-llvm

#LDFILE= test.ld
LDFILE= tmp.ld
MD_LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=nano.specs -T $(LDFILE) -Wl,-Map=middle.map
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=nano.specs -T $(LDFILE) -Wl,-Map=final.map
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall
STDLIB= -I /usr/lib/arm-none-eabi/include/

INFORM_PATH = information/
PY_PATH = py_program




#########################################################
# final -> linking and make .elf file #
final: pass6 final.elf

final.elf: output_opt5.o startup.o syscalls.o
	arm-none-eabi-gcc $(LDFLAGS) -o $@ $^

###########################################################
# pass6 -> Set Arguments #
pass6: pass5 output_opt5.txt output_opt5.ll

output_opt5.txt: output_opt5.o
	arm-none-eabi-objdump -d $^ > $@

output_opt5.o: output_opt5.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

output_opt5.ll: output_opt5.bc
	llvm-dis $^ -o $@

output_opt5.bc: output_opt4.bc
	$(OPT_COMMAND) $(OFLAGS) -load $(LIB5) $(pass5) $^ -o $@


###########################################################
# pass5 -> Check Function size #
pass5: pass4 
	python3 $(PY_PATH)/function_size.py 	

###########################################################
# pass4 -> Insert read addr/size and MPU Config#
pass4: pass3 output_opt4.txt output_opt4.ll

output_opt4.txt: output_opt4.o
	arm-none-eabi-objdump -d $^ > $@

output_opt4.o: output_opt4.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

output_opt4.ll: output_opt4.bc
	llvm-dis $^ -o $@

output_opt4.bc: output_opt3.bc
	$(OPT_COMMAND) $(OFLAGS) -load $(LIB4) $(pass4) $^ -o $@


###########################################################
# pass3 -> Insert MPU Config (function start / before call / behind call)#
pass3: pass2 output_opt3.txt output_opt3.ll

output_opt3.txt: output_opt3.o
	arm-none-eabi-objdump -d $^ > $@

output_opt3.o: output_opt3.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

output_opt3.ll: output_opt3.bc
	llvm-dis $^ -o $@

output_opt3.bc: output_opt2.bc
	$(OPT_COMMAND) $(OFLAGS) -load $(LIB3) $(pass3) $^ -o $@

###########################################################
# pass2 -> BB label #
pass2: pass1 output_opt2.txt output_opt2.ll

output_opt2.txt: output_opt2.o
	arm-none-eabi-objdump -d $^ > $@

output_opt2.o: output_opt2.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

output_opt2.ll: output_opt2.bc
	llvm-dis $^ -o $@

output_opt2.bc: output_opt1.bc
	$(OPT_COMMAND) $(OFLAGS) -load $(LIB2) $(pass2) $^ -o $@

###########################################################
# pass1 -> make Function list in /information directory (function.txt) #
pass1: output_opt1.txt output_opt1.ll startup.o syscalls.o

output_opt1.txt: output_opt1.o
	arm-none-eabi-objdump -d $^ > $@

output_opt1.o: output_opt1.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

output_opt1.ll: output_opt1.bc
	llvm-dis $^ -o $@

output_opt1.bc: output.bc
	$(OPT_COMMAND) $(OFLAGS) -load $(LIB1) $(pass1) $^ -o $@

output.ll: output.bc
	llvm-dis $^ -o $@

####compress####
output.bc: main.bc mpu_configure.bc cnt.bc 
	llvm-link -o $@ $^

cnt.bc: cnt.c
	$(CC) $(TRIPLE) $(FLAGS) -c $^ -o $@ $(STDLIB)

############


main.bc: main.c
	$(CC) $(TRIPLE) $(FLAGS) -c $^ -o $@ $(STDLIB)

mpu_configure.bc: mpu_configure.c
	$(CC) -O3 $(TRIPLE) $(FLAGS) -c $^ -o $@ $(STDLIB)

startup.bc: startup.c
	$(CC) $(TRIPLE) $(FLAGS) -c $^ -o $@ $(STDLIB)

syscalls.bc: syscalls.c
	$(CC) $(TRIPLE) $(FLAGS) -c $^ -o $@ $(STDLIB)

startup.o: startup.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@
                                     

syscalls.o: syscalls.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

clean:
	rm -rf *.bc *.ll *.o *.elf *.map output*.txt .*.dot
	rm -rf information/addr.txt information/BB_size.txt information/expect_BB.txt information/middle_BB.txt information/final_BB.txt information/branch.txt
