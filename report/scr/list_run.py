import os

input_dir = '../input/'
output_dir = '../output/'

for j in range(54):
    print(j)
    for i in range(6):
        os.system('./main < ' + input_dir + 't'+str(i) + str(j) +
                  '.txt >> '+output_dir+'m'+str(i)+'.txt')
