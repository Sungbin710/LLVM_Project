CC = clang

OPT_COMMAND = /home/sblee/llvm-project/build/bin/opt
OFLAGS = -enable-new-pm=0
LIB1 = /home/sblee/llvm-project/build/lib/BB_label.so
pass1 = -bb_label
LIB2 = /home/sblee/llvm-project/build/lib/Insert_Function.so
pass2 = -insert_function
LIB3 = /home/sblee/llvm-project/build/lib/Set_Operand.so
pass3 = -set_operand
LIB4 = /home/sblee/llvm-project/build/lib/Insert_Nop.so
pass4 = -insert_nop

MACH= cortex-m4
TRIPLE= --target=thumbv7em-unknown-none-eabi
FLAGS= -mcpu=$(MACH) -mfloat-abi=soft -emit-llvm

LDFILE= tmp.ld
MD_LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=nano.specs -T $(LDFILE) -Wl,-Map=middle.map
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=nano.specs -T $(LDFILE) -Wl,-Map=final.map
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall
STDLIB= -I /usr/lib/arm-none-eabi/include/

INFORM_PATH = information/
PY_PATH = py_program


#########################################################
# final -> linking and make .elf file #
final: pass9 final.elf

final.elf: output_opt4.o startup.o syscalls.o
	arm-none-eabi-gcc $(LDFLAGS) -o $@ $^

#########################################################
# pass9 -> create new linker_script "tmp.ld" (insert section info)  #
pass9: pass8
	python3 $(PY_PATH)/Insert_ld.py

#########################################################
# pass8 -> insert Nop  #
pass8: pass7 output_opt4.txt output_opt4.ll

output_opt4.txt: output_opt4.o
	arm-none-eabi-objdump -d $^ > $@

output_opt4.o: output_opt4.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

output_opt4.ll: output_opt4.bc
	llvm-dis $^ -o $@

output_opt4.bc: output_opt3.bc
	$(OPT_COMMAND) $(OFLAGS) -load $(LIB4) $(pass4) $^ -o $@

#########################################################
# pass7 ->  #
pass7: pass6
	python3 $(PY_PATH)/BB_check2.py

#########################################################
# pass6 -> Set MPU arguments (start_addr, region_size, rgion_num) #
pass6: pass5 output_opt3.txt output_opt3.ll

output_opt3.txt: output_opt3.o
	arm-none-eabi-objdump -d $^ > $@

output_opt3.o: output_opt3.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

output_opt3.ll: output_opt3.bc
	llvm-dis $^ -o $@

output_opt3.bc: output_opt2.bc
	$(OPT_COMMAND) $(OFLAGS) -load $(LIB3) $(pass3) $^ -o $@

#########################################################
# pass5 -> expect BB_size  #
pass5: pass4
	python3 $(PY_PATH)/expect_BB.py
	
#########################################################
# pass4 -> extract current BB size from output_file #
pass4: pass3 
	python3 $(PY_PATH)/BB_check1.py

#########################################################
# pass3 -> config function insert #
pass3: pass2 output_opt2.txt output_opt2.ll

output_opt2.txt: output_opt2.o 
	arm-none-eabi-objdump -d $^ > $@

output_opt2.o: output_opt2.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

output_opt2.ll: output_opt2.bc
	llvm-dis $^ -o $@

output_opt2.bc: output_opt1.bc
	$(OPT_COMMAND) $(OFLAGS) -load $(LIB2) $(pass2) $^ -o $@

#########################################################
# pass2 -> cfg_extract #
pass2: pass1 extract 
extract: output_opt1.ll
	$(OPT_COMMAND) -analyze -dot-cfg $(OFLAGS) $^
	python3 $(PY_PATH)/cfg_extract.py


###########################################################
# pass1 -> BB label #
pass1: output_opt1.txt output_opt1.ll

output_opt1.txt: output_opt1.o 
	arm-none-eabi-objdump -d $^ > $@

output_opt1.o: output_opt1.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

output_opt1.ll: output_opt1.bc
	llvm-dis $^ -o $@

output_opt1.bc: output.bc
	$(OPT_COMMAND) $(OFLAGS) -load $(LIB1) $(pass1) $^ -o $@

output.bc: main.bc mpu_configure.bc
	llvm-link -o $@ $^

startup.o: startup.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

syscalls.o: syscalls.bc
	llc -march=thumb -mcpu=cortex-m4 -filetype=obj $^ -o $@

main.bc: main.c
	$(CC) $(TRIPLE) $(FLAGS) -c $< -o $@ $(STDLIB)

mpu_configure.bc: mpu_configure.c
	$(CC) $(TRIPLE) $(FLAGS) -c $< -o $@ $(STDLIB)

startup.bc: startup.c
	$(CC) $(TRIPLE) $(FLAGS) -c $< -o $@ $(STDLIB)

syscalls.bc: syscalls.c
	$(CC) $(TRIPLE) $(FLAGS) -c $< -o $@ $(STDLIB)


clean:
	rm -rf *.bc *.ll *.o *.elf *.map output*.txt .*.dot tmp.ld
	rm -rf information/addr.txt information/BB_size.txt information/expect_BB.txt information/BB_size2.txt information/branch.txt
