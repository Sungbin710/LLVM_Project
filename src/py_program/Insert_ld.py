function_list = "/home/sblee/llvm-project/build/basic_block/information/function.txt"
function_size_file = "/home/sblee/llvm-project/build/basic_block/information/function_size.txt"
linker_script = "/home/sblee/llvm-project/build/basic_block/test.ld"
created_linker_script = "/home/sblee/llvm-project/build/basic_block/tmp.ld"

function_start = "0x08004000"
link_function_section = ".function" + function_start +" :"

# read Funtion list #
Funcs = []
f = open(function_list, "r")
function_num = 0
lines = f.readlines()
for line in lines:
	if line == '\n':
		break
	line = line.strip()
	Funcs.append(line)
	function_num += 1

f.close()

# read linker script #
line_num = 0
f = open(linker_script, "r")
lines = f.readlines()
for line in lines:
    line_num  += 1
    if link_function_section in line:
        #print(line)
        break

f.close()


#align = "   . = ALIGN(32);\n"

f = open(linker_script, "r")
prev_lines = f.readlines()[:line_num+1]
f.close()

f = open(linker_script, "r")
next_lines = f.readlines()[line_num+1:]
f.close()


add_lines = []
for i in range(function_num):
#    add_lines.append(align)
    f_section = "	" + Funcs[i] + " += .;\n"
    add_lines.append(f_section)

f = open(created_linker_script,"w")
lines = []
lines.append(prev_lines)
lines.append(add_lines)
lines.append(next_lines)

for line in lines:
    for string in line:
        f.write(string)
        #print(string)


