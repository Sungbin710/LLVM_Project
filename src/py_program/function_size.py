output_txt = "/home/sblee/llvm-project/build/basic_block/output_opt4.txt"
function_list = "/home/sblee/llvm-project/build/basic_block/information/function.txt"
function_size_file = "/home/sblee/llvm-project/build/basic_block/information/function_size.txt"
Link_addr_file = "/home/sblee/llvm-project/build/basic_block/information/Link_addr.txt"


class Func:
	def __init__(self):
		self.name = ''
		self.function_size = 0
		self.region_start = 0
		self.region_size = 0

	def print(self):
		print("func name: " + self.name)
		print("function size: " + str(self.function_size))
		print("region_start: " + str(self.region_start) + "(" + str(hex(self.region_start))  + ")")
		print("region_size: " + str(self.region_size))
		print()

def region_size(size):
    mini = 32
    while size > mini:
        mini = mini*2

    return mini


funcs = []
f = open(function_list, "r")
lines = f.readlines()
for line in lines:
	if line == '\n':
		break
	func = Func()
	func.name = line.strip()
	funcs.append(func)
f.close()

list_num = len(funcs)


i = 0

while i < list_num:

	check = 0
	line_num = 0
	first_line = 0
	last_line = 0

	# find function
	f_name = "<"+funcs[i].name+">:"

	f = open(output_txt,"r")
	lines = f.readlines()
	for line in lines:
		line_num += 1
		if f_name in line:
			break
	f.close()

	#================================================#
	first_line = line_num
	offset = line_num
	last_line = offset
	line_num = 0
	
	# specify function region
	f = open(output_txt,"r")
	lines = f.readlines()[offset:]
	for line in lines:
		line_num += 1
		if line == '\n':
			break
		if line == lines[len(lines)-1]:
			line_num += 1
			break
	f.close()

	last_line += line_num
	#print(lines[first_line])
	#print(lines[last_line])

	f = open(output_txt,"r")
	lines = f.readlines()[first_line:last_line]
	
	tmp = lines[0].split(":",maxsplit=1)[0].strip()
	start_addr = int(tmp, 16) 
	#print(start_addr)
	
	last = len(lines)
	tmp = lines[last-2].split(":",maxsplit=1)[0].strip()
	end_addr = int(tmp,16)
	#print(end_addr)

	funcs[i].function_size = (int(end_addr)-int(start_addr)+2)


	i += 1

#for i in range(list_num):
#	print(funcs[i].function_size)

funcs = sorted(funcs,key=lambda self: self.function_size, reverse=True)

f = open(function_size_file,'w+')
for i in range(len(funcs)):
	f.write(funcs[i].name + ": " + str(funcs[i].function_size) + "\n")

f.close()

######################################################
# Link_addr.txt #

CODE_start = "0x08004000"
funcs[0].region_start = int(CODE_start,16)
for i in range(list_num):
	funcs[i].region_size = region_size(funcs[i].function_size)
	if i < list_num-1:
		funcs[i+1].region_start = funcs[i].region_start + funcs[i].region_size


f = open(Link_addr_file,'w+')
for i in range(len(funcs)):
	f.write("func name: " + funcs[i].name + '\n')
	f.write("function size: " + str(funcs[i].function_size) + '\n')
	f.write("start_region: " + str(funcs[i].region_start) + " (" + str(hex(funcs[i].region_start))  + ")" + '\n')
	f.write("region_size: " + str(funcs[i].region_size) + '\n')
	f.write('\n')
f.close()

#######################################################
#for i in range(list_num):
#	funcs[i].print()


linker_script = "/home/sblee/llvm-project/build/basic_block/test.ld"
created_linker_script = "/home/sblee/llvm-project/build/basic_block/tmp.ld"


function_start = "0x08004000"
link_function_section = ".function " + function_start +" :"


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

tmp = 0
for i in range(list_num):
	if funcs[i].name == "main":
		f_section = "	*(.text.*)\n"	
		add_lines.append(f_section)
		align = "	. = ALIGN(" + str(funcs[i].region_size)  + ");\n"
		add_lines.append(align)
		tmp = 1
		continue

	f_section = "	*(.f" + str(i+1-tmp) + ")\n"
	add_lines.append(f_section)
	align = "	. = ALIGN(" + str(funcs[i].region_size)  + ");\n"
	add_lines.append(align)

f = open(created_linker_script,"w+")
lines = []
lines.append(prev_lines)
lines.append(add_lines)
lines.append(next_lines)

for line in lines:
    for string in line:
        f.write(string)
        #print(string)

