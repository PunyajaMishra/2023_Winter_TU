import matplotlib.pyplot as plt
import numpy as np

# Load data from file
data = np.loadtxt('data.txt')

# Plot with points and color based on 3rd column
plt.scatter(data[:,0], data[:,1], c=data[:,2], cmap='viridis')

# Add colorbar
plt.colorbar()

# Show the plot
plt.show()
