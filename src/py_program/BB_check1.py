output_txt = "/home/sblee/llvm-project/build/basic_block/output_opt5.txt"
function_list = "/home/sblee/llvm-project/build/basic_block/information/function.txt"
BB_size_txt = "/home/sblee/llvm-project/build/basic_block/information/BB_size.txt"


class Func:
	def __init__(self):
		self.name = ''
		self.BB_cnt = 0
		self.BB_size = []

	def print(self):
		print("func name: " + self.name)
		for i in range(self.BB_cnt):
			print("BB" + str(i+1) + " size: " + str(self.BB_size[i]))
		print()


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
	#print(first_line)
	#print(last_line)
	#print(lines[first_line])
	#print(lines[last_line])


	#================================================#
	# divide by BB
	f = open(output_txt,"r")
	lines = f.readlines()[offset:last_line-1]
	BB_START = 0
	BB_END = 0

	for line in lines:

		tmp = line.split(':',maxsplit=1)
		#addr = int(tmp[0].strip(), 16)
		tmp = tmp[1]
		tmp = tmp.split(maxsplit=5)
		#print(tmp)
		br = tmp[1]

		if br == 'b.n' or br == 'bx' or br == 'pop' or tmp[2] == 'b.w':
		#if br == 'e7ff' or br == '4770' or br == 'bdf0' or br == 'f000':
			tmp = lines[BB_START]
			tmp = tmp.split(':', maxsplit=1)
			start_addr = int(tmp[0].strip(), 16)
			tmp = lines[BB_END]
			tmp = tmp.split(':', maxsplit=1)
			end_addr = int(tmp[0].strip(), 16)

			funcs[i].BB_cnt += 1
			funcs[i].BB_size.append(end_addr - start_addr + 2)
			BB_END += 1
			BB_START = BB_END
			continue

		BB_END += 1

	f.close()

	i += 1

f = open(BB_size_txt,'w+')
for i in range(len(funcs)):
	f.write("func name: " + funcs[i].name + '\n')
#f.write(str(funcs[i].BB_cnt) + '\n')
	for j in range(funcs[i].BB_cnt):
		f.write("BB" + str(j+1) + " size: " + str(funcs[i].BB_size[j]) + '\n')
	f.write('\n')

f.close()

