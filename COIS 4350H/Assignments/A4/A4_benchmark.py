import timeit
import timeit
import random

MAX_COUNT = 100000000

print("Creating file of 100MB")
#start_time = timeit.default_timer()
myfile1 = open("myfile1","w")
for i in range(MAX_COUNT):
    myfile1.write(str(random.randint(0,9)))

#print("Creation of 100MB file takes", timeit.default_timer() - start_time, " time")
print("Reading and copying contents now from the file of 100MB")
copy_list = []
myfile2 = open("myfile1","r")
start_time = timeit.default_timer()
copy_list.append(myfile2.readline())
#print("Copying contents from 100MB file takes", timeit.default_timer() - start_time, " time")