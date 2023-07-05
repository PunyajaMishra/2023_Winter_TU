# ------------------------------------------------------------------------------
# COIS-4470H Modelling and Simulation
# Assignment 3 Question 3f
# Name : GAP Test
# 
# Written By : Punyaja Mishra, March 2023
#
# Language : Python
#  
# This program performs GAP Test on the randomly generated 100 variates
#        x_i+1 = g(x) = a * x_i mod 251 for all i >= 1
#
# Input File :
#       None

# -----------------------------------------------------------------------------


import math
from scipy.stats import chi2        # Required to access the "theoretical" chi value of table

# the fiunciton generates a multiplier value
def get_period_multiplier(mod: int) -> int:
    a = 1
    while True:
        p = 1
        x = a
        while x != 1:
            p += 1
            x = (a * x) % mod
        if p == mod - 1:
            return a
        a += 1

# uniformly distributed random numbers are generated
def generate_random_numbers(seed: int, a: int, m: int, n: int) -> list:
    numbers = []
    x = seed
    for i in range(n):
        x = (x * a) % m
        numbers.append(round(x / m,2))          # rounding to 2 decimal places
    return numbers

# perform GAP Test
def gap_test(random_numbers:list, a: float, b:float):
    num_count = 0           # count the number of random numebr that fall in the range
    gap_length = {}
    gap = 0

    # we go through each random number and check if it lies within the range (0.2, 0.5)
    # if the number is in range and it is not the first number - 
    #       - we check if the gap length (i) already exists in the dict
    #       - if not then we create new value else we increment the number by 1
    #   we set gap to 0 again to calculate new gap
    # if the number is not in range then it is a 'gap' so gap goes ++
    for num in random_numbers:
        if a < num < b:
            num_count += 1
            if num_count!=1:
               if gap in gap_length:
                   gap_length[gap] += 1
               else:
                   gap_length[gap] = 1
            gap = 0
        else:
            gap += 1
        last_number = num 
    
    return num_count, gap_length

# calculating chi square value
def chi_sq_test(numerator: list, denominator: list)->float:
    chi_value = 0
    for i in range(len(numerator)):
        chi_value += (numerator[i]**2)/denominator[i]
    return chi_value

# main
if __name__ == '__main__':
    x0 = int(input("\n Please enter seed 'x0' value: "))
    n = 100
    m = 251
    alpha = 0.01
    a = get_period_multiplier(m)

    random_numbers = generate_random_numbers(x0,a,m,n)
 
    # Apply the Gap Test with interval (0.2, 0.5) and level of significance of 0.01
    a = 0.2
    b = 0.5
    length = b-a

    # perform gap test and get the gaps, and gap lengths
    num_count, gap_length = gap_test(random_numbers, a, b)
    print("\n Random Numbers in range: ", num_count)
    print("\n Number of gaps : ", (num_count-1))
    print("\n Printing Gap length and fo: \n")
    print(gap_length)

    expected= []
    diff_obsexp = []

    # get the f_e and (f_o - f_e) values
    for i in gap_length:
        # get f_e values
        expected_val = length* (1-length)**i * (num_count-1)
        expected.append(expected_val)
        # calculate the f_o - f_e
        difference = gap_length[i] - expected_val
        diff_obsexp.append(difference)

    print("\n Printing fe values: \n")
    print(expected)

    # calulcate X2 test
    chi_value = chi_sq_test(diff_obsexp,expected)
    print("\n X2 chi : ", chi_value)

    # theoritical chi value
    chi_theory_value = chi2.ppf(1-alpha, (num_count-1))
    print("\n Table value: ", chi_theory_value)

    # check if independent
    if chi_value < chi_theory_value:
        print("\n Random Numbers are independent!")
    else:
        print("\n Random Numbers are no independent!")

    
    print("Generating random numbers uniformly from (0,1)\n")
    for num in random_numbers:
        print(num)