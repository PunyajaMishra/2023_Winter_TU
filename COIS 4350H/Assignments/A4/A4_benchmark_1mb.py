import timeit
import timeit
import random

MAX_COUNT = 24
MB_1 = 1000000

print("Creation 24 1 MB file and writing in them....")
print("cprofiling...")
#start_time = timeit.default_timer()
for i in range(MAX_COUNT):
    myfile1 = open(("file_"+str(i)),"w")
    for j in range(MB_1):
        myfile1.write(str(random.randint(0,9)))

#print("Creation and writing into 24s 1 MB takes", timeit.default_timer() - start_time, " time")
