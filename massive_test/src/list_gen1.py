import random

outfile = open('massive_test/input/t'+str(0)+'.txt', 'w')
outfile.write(str(0)+"\n")
outfile.write(str(1000)+"\n")

for x in range(1000):
    y = random.randint(1, 1000)
    outfile.write(str(y)+'\n')
