import sys
import random
import os
import time


# In[7]:


TEST_SIZE = 6
PER_SIZE = 5
INT_MAX = 100
size = [100, 300, 600, 900, 1200, 1500]
if __name__ == "__main__":
    dirname = '../inputs'
    if not os.path.exists(dirname):
        os.makedirs(dirname)
    for cases in range(TEST_SIZE):
        for i in range(PER_SIZE):
            filename = '../inputs/{}{}.in'.format(cases, i)
            with open(filename, 'w') as w:
                n = size[cases]
                w.write(str(n) + '\n' + str(n) + '\n')
                w.write('0 0\n')
                w.write(str(n-1) + ' ' + str(n-1) + '\n')
                for x in range(n):
                    for y in range(n):
                        w.write(str(random.randint(0, INT_MAX-1)) + ' ')
                    w.write('\n')
            print("Cases{}{}".format(cases, i))
