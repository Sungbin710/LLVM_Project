function_list = "/home/sblee/llvm-project/build/basic_block/information/function.txt"
expect_size_file = "/home/sblee/llvm-project/build/basic_block/information/expect_BB.txt"
output_file = "/home/sblee/llvm-project/build/basic_block/information/Addr_info.txt"
function_start = 0x08004000
f_section = ".function_section"

class Func:
    def __init__(self):
        self.name = ''
        self.start_addr = 0
        self.BB_cnt = 0
        self.BB_size = []
        self.BB_addr = []

    def print(self):
        print("func name : " + self.name)
        for i in range(self.BB_cnt):
            print("BB" + str(i+1) + " addr: " + str(self.BB_addr[i]))
            print("BB" + str(i+1) + " size: " + str(self.BB_size[i]))
        print()



# read function list : function_num
Funcs = []
line_num = 0
f = open(expect_size_file, "r")
lines = f.readlines()
for line in lines:
    line_num += 1
f.close()

start_line = 0
while line_num-1 >= start_line:

	name_line = start_line

	f = open(expect_size_file, "r")
	lines = f.readlines()[name_line:]
	for line in lines:
		if "func name" in line:
			func = Func()
			tmp = line.split(':' , maxsplit=1)[1].strip()
			func.name = tmp
			break
		name_line += 1
	f.close()

	f = open(expect_size_file, "r")
	lines = f.readlines()[name_line+1:]
	for line in lines:
		name_line += 1
		if line == '\n':
			break
		func.BB_cnt += 1
		tmp = line.split(':' , maxsplit=1)[1].strip()
		func.BB_size.append(int(tmp))

	start_line = name_line
	Funcs.append(func)


Funcs[0].start_addr = function_start

for i in range(len(Funcs)):
	Funcs[i].BB_addr.append(Funcs[i].start_addr)
	
	for j in range(1,Funcs[i].BB_cnt):
		bb_addr = Funcs[i].BB_addr[j-1] + Funcs[i].BB_size[j-1]
		Funcs[i].BB_addr.append(bb_addr) 

	if i != len(Funcs)-1:
		f_addr = Funcs[i].BB_addr[Funcs[i].BB_cnt-1] + Funcs[i].BB_size[Funcs[i].BB_cnt-1]
		Funcs[i+1].start_addr = f_addr
'''
for i in range(len(Funcs)):
	Funcs[i].print()
'''

f = open(output_file, "w+")
for i in range(len(Funcs)):
	f.write("func name: " + Funcs[i].name + "\n")
	for j in range(Funcs[i].BB_cnt):
		f.write("BB" + str(j+1) + " addr: " + str(hex(Funcs[i].BB_addr[j])) + "(" + str(Funcs[i].BB_addr[j]) + ")"'\n')
		f.write("BB" + str(j+1) + " size: " + str(Funcs[i].BB_size[j]) + '\n')
	f.write('\n')
