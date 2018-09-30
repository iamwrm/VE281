import random

line_count = 10

for i in range(6):
    for j in range(1):
        outfile = open('massive_test/input/t'+str(i)+str(j)+'.txt', 'w')
        outfile.write(str(i)+"\n")
        outfile.write(str(line_count)+"\n")
        for x in range(line_count):
            y = random.randint(1, line_count)
            outfile.write(str(y)+'\n')
