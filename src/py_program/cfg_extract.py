#function_list_file = "information/function.txt"
function_list_file = "/home/sblee/llvm-project/build/basic_block/information/function.txt"
path = "/home/sblee/llvm-project/build/basic_block/"

class Func:
    def __init__(self):
        self.name = ''
        self.BB_name = []
        self.BBIR_addr = []
        self.BB_cnt = 0
        self.BB_size = []
        self.next_BB = []

    def print(self):
        print("func name: " + self.name)
        print("BB_cnt: " + str(self.BB_cnt))
        for i in range(len(self.BB_name)):
            print(self.BB_name[i] + ": " + self.BBIR_addr[i])
            print(self.name + "_BB" + str(i+1) + " -> " + str(self.next_BB[i]))
        print()


funcs = []
f = open(function_list_file, "r+")
lines = f.readlines()
for line in lines:
    if line == '\n':
        break
    func = Func()
    func.name = line.strip()
    funcs.append(func)
f.close()


# BB_name과 BB_address Mapping
for i in range(len(funcs)):
    cfg_file = path + "." + funcs[i].name + ".dot"
    f = open(cfg_file, "r")
    lines = f.readlines()
    for line in lines:
        if "Node" in line:
            label_index = line.find("label")
            if label_index != -1:
                # Node addr 추가
                Node = line.split('[',maxsplit=1)[0].strip()    # BB address
                funcs[i].BBIR_addr.append(Node)

                # BB name 추가
                BB_index = label_index + len("label=\"{")
                BB_line = line[BB_index:BB_index+len(funcs[i].name) + len("_BB") + 3]    # BB 999개 까지
                BB_label = BB_line.split(':',maxsplit=1)[0]
                funcs[i].BB_name.append(BB_label)
    funcs[i].BB_cnt = len(funcs[i].BB_name)     # cnt 추가 작업
    #funcs[i].print()
    f.close()


# cfg 추가

for i in range(len(funcs)):
    cfg_file = "." + funcs[i].name + ".dot"
    total_nexts = []
    for t in range(int(funcs[i].BB_cnt)):
        tmp = []
        total_nexts.append(tmp)

    f = open(cfg_file, "r")
    lines = f.readlines()
    for line in lines:


        if "->" in line:
            # Node0x62950c0:s0 -> Node0x6295d20;

            # Node0x62950c0:s0
            curr = line.split("->",maxsplit=1)[0]
            # Node0x6295d20;
            next = line.split("->",maxsplit=1)[1]

            #Node0x62950c0
            curr = curr.split(":",maxsplit=1)[0].strip()
            #Node0x6295d20
            next = next.split(";",maxsplit=1)[0].strip()

            curr_bb_index = funcs[i].BBIR_addr.index(curr)
            next_bb_index = funcs[i].BBIR_addr.index(next)
            #print(funcs[i].BBIR_addr.index(curr))
            #print(funcs[i].BBIR_addr.index(next))

            next_bb = funcs[i].BB_name[next_bb_index]
            total_nexts[curr_bb_index].append(next_bb)

#total_nexts[curr_bb_index].append(next_bb_index+1)

    funcs[i].next_BB = total_nexts
    f.close()

#path = "/home/sblee/llvm-project/build/basic_block/"
f = open(path+"information/branch.txt","w+")
for i in range(len(funcs)):
    f.write("func name: " + funcs[i].name +"\n")
    f.write(funcs[i].name + "_BB_cnt: " + str(funcs[i].BB_cnt) + "\n")
    for z in range(funcs[i].BB_cnt):
        f.write(funcs[i].BB_name[z] + ": " + str(len(funcs[i].next_BB[z])) + "\n")
        for k in range(len(funcs[i].next_BB[z])):
            f.write(" -> " + str(funcs[i].next_BB[z][k]) + "\n")
    f.write('\n')

