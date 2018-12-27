import random

num = 2000
for i in range(100):
    outfile = open('../p5/input/t'+str(i)+'.txt', 'w')
    outfile.write(str(num)+"\n")
    for j in range(num-1):
        # i_from = random.randint(0, num-1)
        i_from = j
        die = random.randint(0, 20)
        if (die < 17):
            i_to = random.randint(j+1, num-1)
        else:
            i_to = random.randint(0, num-1)

        i_weight = random.randint(0, 20)
        outfile.write(str(i_from)+' ' + str(i_to)+' ' + str(i_weight)+'\n')
