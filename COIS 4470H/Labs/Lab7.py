import numpy as np
import matplotlib.pyplot as plt

samples = list()
def generateValues():
    u = np.random.uniform(0,1)

    if(u<(1/6)):
        x = 1
    elif(u<(1/2)):
        x = 2
    elif(u<(3/4)):
        x = 3
    else:
        x = 4

    return x


for i in range(1000):
    samples.append(generateValues())

plt.hist(samples,density=True,bins=[0.5,1.5,2.5,3.5,4.5],color='blue',rwidth=0.8)
plt.xlabel('Random Values')
plt.ylabel('Frequency')
plt.title('Frequency of Random Numbers Generated')
plt.show()
    
