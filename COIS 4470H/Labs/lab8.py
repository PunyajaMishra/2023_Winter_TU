import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import math
import openpyxl

mu = 100
LAST = 1000                  
START = 0.0                   
M = 500      
P = [0.1, 0.2, 0.5, 0.7, 0.9]              

class Sum: wait, delay, service, interarrival = 0.0, 0.0, 0.0, 0.0

# to calculate failure time - mean 100
# generate exponential variates
def get_failure():
    u = np.random.uniform(0,1)
    x = -mu * np.log(1-u)
    return x

def next_failure(failure):
    return np.amin(failure)

# number of tasks required
# Function to generate random variates following a geometric distribution wi
#def get_tasks_geometric(p):
 #   u = np.random.uniform(0, 1)
  #  x = int(np.ceil(np.log(1 - u)/np.log(1 - p))) - 1
   # return x

# uniform
#def get_service():
 #   return np.random.uniform(10, 20)

def quotient(a:int, b:int):
    return a / b if b != 0 else 0.0

def main(p):
    index, arrival, delay, service, wait, departure, n_tasks, t_tasks = 0, START, 0.0, 0.0, 0.0, START, 0, 0                     
    failure = np.array([START + get_failure() for _ in range(M)])
    sum = Sum()

    while index < LAST:
        index += 1
        arrival = next_failure(failure)
        delay = departure - arrival if arrival < departure else 0.0
        n_tasks = np.random.geometric(p) + 1
        t_tasks = np.random.uniform(10, 20, size=n_tasks).sum()
        service = t_tasks
        wait = delay + service
        departure = arrival + wait
        m = np.argmin(failure)
        failure[m] = departure + get_failure()
        sum.wait += wait
        sum.delay += delay
        sum.service += service
    sum.interarrival = arrival - START
    return [quotient(sum.delay, index), quotient(sum.service, index), quotient(sum.service, departure) * 100]

results = {p: main(p) for p in P}
df = pd.DataFrame.from_dict(results, orient='index', columns=['Average delay time', 'Average service time', 'Server utilization'])
df.index.name = 'P'
print(df)
        
df.to_excel("lab8_m500.xlsx",sheet_name='50')
