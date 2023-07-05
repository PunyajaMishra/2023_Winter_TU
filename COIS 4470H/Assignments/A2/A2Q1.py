# ------------------------------------------------------------------------------
# COIS-4470H Modelling and Simulation
# Assignment 2 Question 1
# Name : Inventory system
# 
# Written By : Punyaja Mishra, February 2023
#
# Language : Python
# 
# This program simulates a simple (s,S) inventory system using demand read  
# from a text file.  Backlogging is permitted and there is no delivery lag.
# The output statistics are the average demand and order per time interval
# (they should be equal), the relative frequency of setup and the time
# averaged held (+) and short (-) inventory levels.
#
# Input File :
#       ssq1.dat 
#
# Holding cost (C_holding) - $25 per car per week
# Shortage cost (C_Shortage) - $700 per car per week
# Set up cost (C_SetUp) - $1000 per order
# Unit cost (C_Unit) - $8000 each car ordered
# -----------------------------------------------------------------------------


FILENAME = "sis1.dat"       #input file
MAXIMUM = 80                # 'S' maximum inventory parameter
MINIMUM = 0                # 's' maximum inventory parameter
C_HOLDING = 25              # Holding Cost
C_SHORTAGE = 700            # Shortage Cost
C_SETUP = 1000              # Setup Cost if Number of Orders <= 70
C_SETUP2 = 1200             # Setup Cost if Number of Orders > 70
C_UNIT = 8000               # Unit cost of Item

# Struct Sum
class sum:
  setup = 0.0           # number of setups - number of orders less than equal to 70
  holding = 0.0         # inventory in holding +
  shortage = 0.0        # inventory in shortage -
  order = 0.0           # amount of orders
  demand = 0.0          # amount of demands
  setup2 = 0.0          # numbers of setups - number of orders greater than 70

###########################Main Program################################

# open the input file with data
fp = open(FILENAME, "r")
demands = [] # array to store the read demands times

# read through file - This is the equivalent of get demand methods in C++, C#, Java
for line in fp:
  demands.append(float(line.split()[0])) #add all rows of first column to demands arrray

fp.close()
# file closed

# Define helper functions
# Square - Math.Pow(x,2)
def sqr(x):
    return x * x
# Divide 2 numbers and return result round up to 2 decimal places
def quotient(a, b):
    return round(a / b, 2)

# define variables
index = 0;                      # time interval index
inventory = MAXIMUM;            # current inventory level - start at Maximum
order = 0                       # Current Number of Items Order - start at 0
demand = 0                      # Current Demand - 0 at time 0
sum = sum()                     # Initialize Object of Sum Class
sad_customers = 0               # unsatisfied custoemrs / unfulfilled demands
additional_demand = 0           # customers from last week who come back 

# Traverse through the number of demands in the file
# There are the number of demands for each time interval, therefore - number of time intervals
for i in range(0,len(demands)):
    index += 1                          # time intervl +=1
    
    # if inventory is less than minimum amount allowed
    if(inventory < MINIMUM):            # place an order
        order = MAXIMUM - inventory     # how much to order
        sum.order += order              # total number of items ordered
        #sum.setup += 1                  # add number of orders placed in system - Backorder
        if(order <= 70):
          sum.setup +=1
        else:
          sum.setup2 +=1
    else:
      order = 0;                        # No order is placed

    inventory += order;                # Backorder is allowed - add up the inventory since there will be holding cost
    #demand = demands[i]                # Current Demand 
    demand = demands[i] + additional_demand                # Current Demand + demands from last week         
    sum.demand += demand                # Total demand of system

    # BACKORDERING ALLOWED
    # inventory is more than demand - holding cost
    #if(inventory > demand):
     #sum.holding += inventory - 0.5*demand
    # shortage cost
    #else:
      #sum.holding += sqr(inventory)/(2.0*demand)
      #sum.shortage += sqr(demand-inventory)/(2.0*demand)

      
    # NO BACKORDERING 
    # inventory is more than demand then all good. Else inventory goes to 0 and there are unsatisfied customers
    if(inventory >= demand):
      sum.holding += inventory - 0.5*demand
      inventory -= demand
    else:
      additional_demand = demand - inventory
      sad_customers += (demand - inventory)          # part e - unsatisfied customers
      inventory = 0

    sum.shortage = 0      # no shortage since no backordering
    
    # update inventory - remove demand from it (backorder allowed)
    # inventory -= demand
   

# stock up to maximum inventory level at the end
# one last order if required
if(inventory < MAXIMUM):
  order = MAXIMUM - inventory
  sum.setup += 1
  sum.order += order
  inventory += order


# Calculate performance metrics and output results
avg_order = quotient(sum.order, index)          # average orders o
avg_item_cost = C_UNIT * avg_order              # average item cost

setup_freq = quotient(sum.setup, index)         # average order freuquency u
avg_setup = round(C_SETUP * setup_freq,2)       # avergae setup cost
setup_freq2 = quotient(sum.setup2, index)       # average order freuquency u - order>70
avg_setup2 = round(C_SETUP2 * setup_freq2,2)    # avergae setup cost - order>70
final_avg_setup = avg_setup + avg_setup2

avg_holding = quotient(sum.holding, index)              # average holding level l+
avg_holding_cost = round(C_HOLDING * avg_holding,2)     # average holding cost

avg_shortage = quotient(sum.shortage, index)            # average shortage level l-
avg_shortage_cost = round(C_SHORTAGE * avg_shortage,2)  # average shortage cost

sum_costs = final_avg_setup + avg_holding_cost + avg_shortage_cost

print(MINIMUM)
#print(f"For {index} time intervals with an average demand of {sum.demand} and policy parameters ({MINIMUM}, {MAXIMUM}),")
#print(f"the average order: {avg_order}")
#print(f"---- Average Item Cost ${avg_item_cost}")
#print(f"the average holding level: {avg_holding}")
print(f"---- Average Holding Cost ${avg_holding_cost}")
#print(f"the average shortage level: {avg_shortage}")
print(f"---- Average Shortage Cost: ${avg_shortage_cost}")
#print(f"the setup frequency: {setup_freq}")
print(f"---- Average setup Cost ${final_avg_setup}")
print(f"---- Sum of 3 Costs: ${sum_costs}")
print(f"---- Number of unsatisfied customers: {sad_customers}")


