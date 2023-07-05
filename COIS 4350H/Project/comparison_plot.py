import matplotlib.pyplot as plt

# Data for each language
python_seq = [0.195697328, 0.987695418, 2.536387072, 11.7239695,27.00649194]
python_par_2 = [0.89783561, 1.56792857, 2.99578299, None, None]
python_par_4 = [0.87643857, 1.18468844, 2.25674926, None, None]
python_par_8 = [0.87324924, 0.99362996, 2.03146682, None, None]
cpp_seq = [0.036389, 0.636839 , 0.991119 , 1.978082 ,3.955956]
cpp_par_2 = [0.009203, 0.16229, 0.34798, 1.636871, 3.716447]
cpp_par_4 = [0.021769, 0.202246, 0.358226, 1.74074, 3.755597]
cpp_par_8 = [0.015597, 0.199292, 0.370482, 1.722188, 3.500614]
julia_seq = [0.0093284, 0.1590509, 0.1651588, 1.7521064, 3.8246149]
julia_par_2 = [0.0319778, 0.0570359, 0.1854954, 0.564479, 1.0019266]
julia_par_4 = [0.0229999, 0.0421051, 0.085571, 0.4935206, 0.9397039]
julia_par_8 = [0.0300136, 0.0640103, 0.1327308, 0.2469843, 3.3964909]

# Number of data points
n = [1000, 5000, 10000, 50000, 100000]

# Create a figure and axis object
fig, ax = plt.subplots()

# Set the axis labels and title
ax.set_xlabel('Number of Data Points')
ax.set_ylabel('Execution Time (s)')
ax.set_title('Comparison of Language Performance')

# Add the data to the plot
ax.plot(n, python_seq, '-o', label='Python (Sequential)')
ax.plot(n, python_par_2, '-o', label='Python (Parallel, 2 Threads)')
ax.plot(n, python_par_4, '-o', label='Python (Parallel, 4 Threads)')
ax.plot(n, python_par_8, '-o', label='Python (Parallel, 8 Threads)')
ax.plot(n, cpp_seq, '-o', label='C++ (Sequential)')
ax.plot(n, cpp_par_2, '-o', label='C++ (Parallel, 2 Threads)')
ax.plot(n, cpp_par_4, '-o', label='C++ (Parallel, 4 Threads)')
ax.plot(n, cpp_par_8, '-o', label='C++ (Parallel, 8 Threads)')
ax.plot(n, julia_seq, '-o', label='Julia (Sequential)')
ax.plot(n, julia_par_2, '-o', label='Julia (Parallel, 2 Threads)')
ax.plot(n, julia_par_4, '-o', label='Julia (Parallel, 4 Threads)')
ax.plot(n, julia_par_8, '-o', label='Julia (Parallel, 8 Threads)')

# Add a legend
ax.legend()

# Show the plot
plt.show()
