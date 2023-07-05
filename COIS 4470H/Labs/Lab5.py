# COIS 4470H Lab 5 - Random Number Generator

# X1,0
# X2,0
# a1
# a2
# m1
# m2

import numpy as np
import matplotlib.pyplot as plt

def random_generator(X1_0,X2_0,a1,a2,m1,m2,TOTAL):
    X1 = X1_0
    X2 = X2_0
    random_numbers = np.zeros(TOTAL)
    #random_numbers = []
    for j in range (TOTAL):
        X1 = (a1 * X1) % m1
        X2 = (a2 * X2) % m2

        X = (X1 - X2) % m1

        R = X1/m1 if X>0 else (m1-1)/m1
        random_numbers[j] = R
        #random_numbers.append(R)

    return random_numbers


#X1_0 = float(input("Enter Value of X1,0: \n"))
X1_0 = 123

#X2_0 = float(input("Enter Value of X2,0: \n"))
X2_0 = 456

#a1 = float(input("Enter Value of a1: \n"))
a1 = 1664525

#m1 = float(input("Enter Value of m1: \n"))
m1 = 2**32

#a2 = float(input("Enter Value of a2: \n"))
a2 = 1013904223

#m2 = float(input("Enter Value of m2: \n"))
m2 = 2**32 - 1

TOTAL = 1000000

generated_random_numbers = random_generator(X1_0,X2_0,a1,a2,m1,m2,TOTAL)


plt.hist(generated_random_numbers, bins=100)
plt.show()
