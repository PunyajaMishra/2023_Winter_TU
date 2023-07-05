use Random;
use Time;
use Math;
use LinearAlgebra;


config const numPoints = 1000;
config const maxIter = 20;
config const k = 20;
config const outFilename = "out.txt";
config const seed1 = 314;
config const seed2 = 294;
config const n = 5;


proc argmin(a: [?D] real): int {
  var minIndex: int = 1;
  for i in 2..a.shape(1) {
    if a[i] < a[minIndex] {
      minIndex = i;
    }
  }
  return minIndex;
}

// function to calculate the Euclidean distance between two points
proc euclideanDistance(a: [1..n] real, b: [1..n] real): real {
  var sum: real = 0.0;
  for i in 1..n do
    sum += (a[i] - b[i]) ** 2;
  return sqrt(sum);
}

// define the k-means clustering function
proc kMeansClustering(data: [1..numPoints, 1..n] real, k: int): [1..numPoints] int {
  // randomly initialize the centroids
  var centroids: [1..k, 1..n] real;
  fillRandom(centroids, seed1, algorithm=RNG.NPB);

  // assign each point to its nearest centroid
  var assignments: [1..numPoints] int;
  var distances: [1..numPoints, 1..k] real;
  for i in 1..numPoints {
    for j in 1..k {
      distances[i, j] = euclideanDistance(data[i, ..], centroids[j, ..]);
    }
    assignments[i] = argmin(distances[i, ..]);
  }

  // repeat until convergence
  var converged: bool = false;
  while !converged {
    // update the centroids to be the mean of the assigned points
    for i in 1..k {
      var points = data[assignments == i, ..];
      if points.shape(1) > 0 {
        centroids[i, ..] = sum(points) / points.shape(1);
      }
    }

    // reassign each point to its nearest centroid
    var newAssignments: [1..numPoints] int;
    var newDistances: [1..numPoints, 1..k] real;
    for i in 1..numPoints {
      for j in 1..k {
        newDistances[i, j] = euclideanDistance(data[i, ..], centroids[j, ..]);
      }
      newAssignments[i] = argmin(newDistances[i, ..]);
    }

    // check for convergence
    converged = (newAssignments == assignments);
    assignments = newAssignments;
    distances = newDistances;
  }

  // return the final assignments
  return assignments;
}

// generate random dataset
var data: [1..numPoints, 1..5] real;
fillRandom(data, seed2, algorithm=RNG.NPB);

// time k-means clustering
var startTime = getCurrentTime();
var assignments = kMeansClustering(data, k);
var endTime = getCurrentTime();
writeln("Execution time:", endTime - startTime, " seconds.");

// write assignments to file
var file = open(outFilename, "w");
for i in 1..numPoints {
  writeln(file, assignments[i]);
}
close(file);
