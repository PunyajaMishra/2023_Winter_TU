# syntax for loading the packages
using Pkg

# to create data
#Pkg.add("Distributions")
using Distributions

# Random to create dataset
#Pkg.add("Random")
using Random

# to create dataframes and load
#Pkg.add("DataFrames")
using DataFrames
  
# to use the above function kmeans()
#Pkg.add("Clustering")
using Clustering
  
# to visualise our clusters formed
#Pkg.add("Plots")
using Plots
  
# RDatasets to load the already made datasets
#Pkg.add("RDatasets")
using RDatasets

# set constants
const NUM_POINT = 5000
const MAX_RANGE = 10000
const NUM_CLUSTER = 15 #20-5dimensions
const MAX_ITERATIONS = 20

# create a random dataset with random numbers
println("Generating Random Dataset")
X = rand(5,NUM_POINT)

println("Performing kmeans")
# cluster X into 20 clusters using K-means
R = kmeans(X, NUM_CLUSTER; maxiter = MAX_ITERATIONS, display=:iter)
@assert nclusters(R) == NUM_CLUSTER # verify the number of clusters

# get the assignments of points to clusters
a = assignments(R)

# get the cluster sizes
c = counts(R)

# get the cluster centers
M = R.centers

#println("Scatter Plot Plotting")
# plot scatter plot
#p = scatter(X[:,1], X[:,2], group=R.assignments, title="kmeans on random dataset", 
 #       xlabel="x", ylabel="y", aspect_ratio=:equal, legend=:bottomright)

# time the execution
#savefig("plot_julia.png")
elapsed_time = @elapsed kmeans(X, NUM_CLUSTER)
println("Time taken to run kmeans: $elapsed_time seconds")
