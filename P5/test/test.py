
# ./main < input/t0.txt > wrout/t0.txt && ./gty-main < input/t0.txt > gtyout/t0.txt && diff wrout/t0.txt  gtyout/t0.txt
import os

for i in range(100):
    wr_main = "../p5/main "
    gty_main = "../p5/gty-main "
    both_input = "< ../p5/input/t"+str(i)+'.txt '
    wr_output = " ../p5/wrout/t"+str(i)+'.txt '
    gty_output = " ../p5/gtyout/t"+str(i)+'.txt '
    andand = "&& "
    diff = "diff "

    wr_part = wr_main+both_input+"> "+wr_output
    gty_part = gty_main+both_input+"> "+gty_output
    os.system(wr_part+andand+gty_part+andand + diff + wr_output+gty_output)
    os.system(gty_main + both_input)
    print(i)

os.system("rm ../p5/input/t*")
os.system("rm ../p5/wrout/t*")
os.system("rm ../p5/gtyout/t*")
