# ------------------------------------------------------------------------------
# COIS-4470H Modelling and Simulation
# Assignment 3 Question 3c,d
# Name : Random Number Generator
# 
# Written By : Punyaja Mishra, March 2023
#
# Language : Python
#  
# This program generates uniformly distrbuted random numbers
#   between 0 and 1 for
#        x_i+1 = g(x) = a * x_i mod 251 for all i >= 1
#
# Input File :
#       None

# -----------------------------------------------------------------------------

import math

# get 'a' 
def get_period_multiplier(mod: int)-> int:
    a = 1

    while True:
        p = 1
        x = a

        while x!=1:
            p += 1
            x = (a * x) % mod
        
        if p==mod-1:
            return a
        
        a += 1

# generate random numbers between 0 and 1
def generate_random_numbers(seed: int, a: int, m: int, n: int) -> list:
    numbers = list()
    x = seed

    for i in range(n):
        x = (x*a) % m
        numbers.append(x/m)
        if x == seed: break
    
    return numbers


# main
if __name__ == '__main__':
    x0 = int(input("\n Please enter seed 'x0' value: "))
    n = 100
    m = 251
    a = get_period_multiplier(m)

    random_numbers = generate_random_numbers(x0,a,m,n)

    print("Generating random numbers uniformly from (0,1): \n")
    for num in random_numbers:
        print(num)