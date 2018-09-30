import os

input_dir = './massive_test/input/'
output_dir = './massive_test/output/'

for i in range(6):
    print(i)
    for j in range(1):
        os.system('./main < ' + input_dir + 't'+str(i) + str(j) +
                  '.txt > '+output_dir+'m'+str(i)+str(j)+'.txt')
        os.system('./true < ' + input_dir + 't'+str(i) + str(j) +
                  '.txt > '+output_dir+'t'+str(i)+str(j)+'.txt')
