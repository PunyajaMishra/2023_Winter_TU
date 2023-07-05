# ------------------------------------------------------------------------------
# COIS-4470H Modelling and Simulation
# Assignment 1 Question 3
# A Single Server Simulation FIFO Service Node
#
# Language : Python
# Written By : Punyaja Mishra, 0660001
#
# This program simulates a single-server FIFO service node using arrival
# times and service times read from a text file. The server is assumed
# to be idle when the first job arrives. All jobs are processed completely
# so that the server is again idle at the end of the simulation. The
# outout statistics are the averge interarrival time, average service
# time, the average delay in the queue, and the average wait in the service
# node
#
# Input File :
#     ssq1.dat (Part c)
#     ssq2.dat (Part b)
# -----------------------------------------------------------------------------


#FILENAME = "ssq2.dat"       # input data file part b                 
FILENAME = "ssq1.dat"       # input data filepart c
START = 0.0 #start time at 0
END = 10000 # end time of the system

# struct sum
# this is to store all delay, wait, service and interarrival times of each person
# to sum them up and calculate the average
class sum:
  delay = 0.0  #delay times
  wait = 0.0 #wait times
  service = 0.0 #service times
  interarrival = -1.0 #interarrival times
  maxDelay = 0.0 # maximum delay

###########################Main Program################################

# open the input file with arrival time and service times 

fp = open(FILENAME, "r")
arrivals = [] # array to store the read arrivals times
services = [] # array to stoer the read service times

# read through file - This is the equivalent of get arrival and get service methods in C++, C#, Java
for line in fp:
  arrivals.append(float(line.split()[0])) #add all rows of first column to arrival time arrray
  services.append(float(line.split()[1])) #add all rows of second column to service time arrray

fp.close()
# file closed

#defining variables of main()
index = 0 # job index            
arrival = START # arrival time of current person         
delay = -1 # delay in queue of current person; starting with -1 so when ++ is done, the first person has 0 delay    
service = -1 # service time; same reason as delay to be -1         
wait = -1 # delay + service      
departure = START # departure time of current person
delayedJobs = 0 #number of jobs delayed

# specified times to know how many jobs in the system
# counters to keep count of number of jobs for these particular times
specifiedTime1 = 2000       # part c
jobsInSystem1 = 0           # part c
specifiedTime2 = 6000       # part c 
jobsInSystem2 = 0           # part c
specifiedTime3 = 300        # part b
jobsInSystem3 = 0           # part c

# creating an object for each arrival of the sum class defined above 
sum = sum() 

print("=== Starting Single Server Simulation for FIFO ===")

# loop through all jobs arrivals
for i in range(0,len(arrivals)):
  index += 1 # number of jobs entered
  arrival      = arrivals[i]  #GetArrival(fp)
  if (arrival < departure): 
    delay      = departure - arrival        # delay in queue
    sum.maxDelay = max(sum.maxDelay, delay) # keep track of the max delay in queue
    delayedJobs+=1
  else: 
    delay      = 0.0                        # no delay          
  service      = services[i] #GetService(fp)
  wait         = delay + service
  departure    = arrival + wait             # time of departure 
  sum.delay   += delay
  sum.wait    += wait
  sum.service += service

  # check if the departure time is greater than the specified time
  # check if the arrival time is less than the specified time
  # If YES, then counter += 1
  if(departure > specifiedTime1 and arrival < specifiedTime1):
    jobsInSystem1+=1

  if(departure > specifiedTime2 and arrival < specifiedTime2):
      jobsInSystem2+=1

  # for part B
#  if(departure > specifiedTime3 and arrival < specifiedTime3):
#      jobsInSystem3+=1

#End of loop

sum.interarrival = arrival - START #interarrival is the arrival time - arrival time of the previosu person 

#printing our results
print("\nfor {0} jobs".format(index))
print("   average interarrival time .. = {0:6.2f} ".format(sum.interarrival / index))
print("   average service time ....... = {0:6.2f} ".format(sum.service / index))
print("   proportions of delayed jobs  = {0:6.2f}%".format((delayedJobs/index)*100))
print("   maximum delay .............. = {0:6.2f} ".format(sum.maxDelay))
print("   average delay .............. = {0:6.2f} ".format(sum.delay / index))
print("   average wait ............... = {0:6.2f} ".format(sum.wait / index))
#printing number of jobs in system at a specified time
print("\n Number of jobs in the system at time {0} is {1}".format(specifiedTime1,jobsInSystem1))
print("\n Number of jobs in the system at time {0} is {1}".format(specifiedTime2,jobsInSystem2))
#printing number of jobs in system at a specified time - part b
#print("\n Number of jobs in the system at time {0} is {1}".format(specifiedTime3,jobsInSystem3))

