import random

base = 1
for i in range(54):
    line_count = int(10**base)
    base = base+0.1
    for j in range(6):
        outfile = open('../input/t'+str(j)+str(i)+'.txt', 'w')
        outfile.write(str(j)+"\n")
        outfile.write(str(line_count)+"\n")
        for x in range(line_count):
            y = random.randint(1, line_count)
            outfile.write(str(y)+'\n')
