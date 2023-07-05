# ------------------------------------------------------------------------------
# COIS-4470H Modelling and Simulation
# Assignment 3 Question 3b
# Name : Full Period Multipliers
# 
# Written By : Punyaja Mishra, March 2023
#
# Language : Python
#  
# This program generates all full period multipliers for
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

# Find all full period multiplers
def find_all_multipliers(mod: int, a: int = None) -> list:

    if a is None:
        a = get_period_multiplier(mod)
    
    i = 1                               # starting with 1 to find all multipliers
    x = a                               # x0 = a
    multpliers = list()

    while x!=1:
        if math.gcd(i,mod-1) == 1:      # if i is a co prime of m then it is a full period multiplier
            multpliers.append(x)
        i += 1                          # i++
        x = (a * x) % mod               #x_i+1 = ax_i % m

    multpliers.sort()
    return multpliers


# main
m = 251
all_multipliers = find_all_multipliers(m)

print("m = ", m)
print("Number of multipliers: ", str(len(all_multipliers)), "\n")
for a in all_multipliers:
    print(a)