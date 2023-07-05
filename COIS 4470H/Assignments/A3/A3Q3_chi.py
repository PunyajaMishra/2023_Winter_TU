# ------------------------------------------------------------------------------
# COIS-4470H Modelling and Simulation
# Assignment 3 Question 3e
# Name : chi squred test
# 
# Written By : Punyaja Mishra, March 2023
#
# Language : Python
#  
# This program performs frequency test on 100 random variates generated
#        x_i+1 = g(x) = a * x_i mod 251 for all i >= 1
#
# Input File :
#       None

# -----------------------------------------------------------------------------

import math
from scipy.stats import chi2        # Required to access the "theoretical" chi value of table

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

# generate uniformly distributed random numbers between 0 and 1
def generate_random_numbers(seed: int, a: int, m: int, n: int) -> list:
    numbers = list()
    x = seed

    for i in range(n):
        x = (x*a) % m
        numbers.append(x/m)
        if x == seed: break
    
    return numbers

# freuquency test
def chi_test(numbers: list, alpha: float, sub_intervals: int) -> bool:
    expected_f = len(numbers)/ sub_intervals        # n/s
    observed_f = [0] * sub_intervals                # f_o_(j)

    for num in numbers:
        interval = int(num * sub_intervals)         # ex: 0.5 * 10 = 5 so sub interval number 5
        observed_f[interval] += 1                   # ++
    
    chi_squared_statistic = 0                       # calculting X2 chi squared statistic
    for freq in observed_f:
        chi_squared_statistic += (freq - expected_f)**2/expected_f

    print("\n chi squared statistic value : ", chi_squared_statistic)

    # get the chi value from table
    degree_of_freedom = sub_intervals - 1           # s-1
    chi_theory_value = chi2.ppf(1-alpha, degree_of_freedom)

    # check if uniform or not
    if chi_squared_statistic < chi_theory_value:
        return True
    else: 
        return False

# main
if __name__ == '__main__':
    x0 = int(input("\n Please enter seed 'x0' value: "))
    n = 100
    m = 251
    alpha = 0.01
    sub_intervals = 10
    a = get_period_multiplier(m)

    random_numbers = generate_random_numbers(x0,a,m,100)
    isUniform = chi_test(random_numbers,alpha,sub_intervals)

    print("Generating random numbers uniformly from (0,1)\n")
    print("USing chi-squared test, Are the random numbers uniformly distributes: ", isUniform)
    for num in random_numbers:
        print(num)
