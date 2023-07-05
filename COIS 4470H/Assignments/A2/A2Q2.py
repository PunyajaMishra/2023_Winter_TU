# ------------------------------------------------------------------------------
# COIS-4470H Modelling and Simulation
# Assignment 2 Question 2
# Name : 3-Die Experiment Monte Carlo Simulation
# 
# Written By : Punyaja Mishra, February 2023
#
# Language : Python
# 
# Three die are rolled and largest of the three up faces is recorded. This program
# uses Monte Carlo Simulation to estimate the probability of each possible value.
# Possible Values are X = 1, 2, 3, 4, 5, 6. Experiment is done for 1000 times
#
# Input File :
#       None
#
#
# EXPERIMENTS = 1000    Number of experiments 
# -----------------------------------------------------------------------------

import random

EXPERIMENTS = 1000              # constant value to store the number of experiments

# Initialize a dictionary to store the probability of each values
# as key: value => X:probability
probabilities = {i:0 for i in range(1, 7)}      # i = 1, 2, 3, 4, 5, 6


# Repeat Experiment number of times
for i in range(EXPERIMENTS):
    
    # 3 die are rolled
    # random number generator generates an integer between 1 and 6
    # this random number generator is called 3 times for 3 die
    # these values are stored in an array called "die"
    die = [random.randint(1,6) for j in range (3)]
    
    # largest of 3 values are recorded  
    largest_value = max(die)
    
    # probability of that value is increases in dictionary
    probabilities[largest_value] += 1

# Probabilities for each possible values of X = [1,6] are printed
# Each value in dictionary is divided by 1000 to get "Probability" between 0 and 1
# round to 2 decimal places
print(f"\nFor {EXPERIMENTS} Experiments: ")
for i in range(1,7):
    print(f"\tProbability of X = {i} is {round(probabilities[i]/1000,2)}")

