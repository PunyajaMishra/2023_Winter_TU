import numpy as np
import matplotlib.pyplot as plt

SAMPLE_SIZE = 1000

# generate exponential variates
def exponential(mu):
    u = np.random.uniform(0,1)
    x = -mu * np.log(1-u)
    return x

# Function to generate random variates following a geometric distribution wi
def geometric(p):
 u = np.random.uniform(0, 1)
 x = int(np.ceil(np.log(1 - u)/np.log(1 - p))) - 1
 return x

mu = float(input("Enter the mean of the exponential distribution: "))
p = float(input("Enter the parameter of the geometric distribution: "))
samples = [exponential(mu) for i in range(SAMPLE_SIZE)]
samples2 = [geometric(p) for i in range(SAMPLE_SIZE)]


# plot 
plt.hist(samples, density=True, bins = 50, alpha=0.7, color='blue')
x = np.linspace(0,10*mu,1000)
y = (1/mu)*np.exp(-x/mu)
plt.plot(x,y,'r--', linewidth=2)

plt.xlabel('Sample Value')
plt.ylabel('Probability Density')
plt.title('Exponential Distribution with Mean={}'.format(mu))
plt.show()

# plot
plt.hist(samples2, bins = 50, density = True, alpha=0.7, color='blue')

# Plot the theoretical PMF of the geometric distribution for comparison
x = np.arange(1, 11)
y = (1-p)**(x-1) * p
plt.stem(x, y, 'r', markerfmt='ro', label='Theoretical PMF')
plt.xlabel('Sample Value')
plt.ylabel('Probability Density')
plt.title('Geometric Distribution with Parameter={}'.format(p))
plt.legend()
plt.show()
