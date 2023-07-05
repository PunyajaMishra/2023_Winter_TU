use Random;
use Time;
use Math;
use IO;

/* Config variables */
config const outFilename = "out.txt";
config const k = 20;
config const maxIter = 20;
config const numPoints = 1000;
config const minCoord = -10.0;
config const maxCoord = 10.0;

/* Datatypes */
type Point = [128] real;

record Cluster {
    var size: int;
    var pointSum: Point;
    var mean: Point;

    proc init() {}

    proc init(point: Point) {
        mean = point;
    }

    proc distance(p: Point) {
        var squaredDiff = (mean - p) ** 2;
        var sum = 0.0;
        for el in squaredDiff {
            sum += el;
        }
        return sqrt(sum);
    }

    proc addPoint(p: Point) {
        pointSum += p;
        size += 1;
    }

    proc setMean(p: Point) {
        mean = p;
    }

    proc calcMean() {
        mean = pointSum / size;
    }
}

proc main() {
    var pointsDomain = {0..#numPoints};
    var points: [pointsDomain] Point;
    var clustersDomain = {0..#k};
    var labels: [pointsDomain] int;
    var clusters: [clustersDomain] Cluster;
    
    /* Generate random dataset */
    var randStream = new owned RandomStream(real);
    for i in pointsDomain {
        var values: [128] real;
        for j in 0..128 {
            values[j] = randStream.getNext();
        }
        points[i] = values;
    }

    var watch: Timer;
    watch.start();
    /* Algorithm */
    for i in clustersDomain {
        var idx: int = (randStream.getNext() * numPoints):int;
        clusters[i] = new Cluster(points[idx]);
    }

    for iteration in 0..#maxIter {
        var newClusters: [clustersDomain] Cluster;
        for i in pointsDomain {
            // You have to be careful with tuples since they create copies
            ref point = points[i];

            var minIndex = 0;
            var minValue = Math.INFINITY;
            for (idx, cluster) in zip(clusters.domain, clusters) {
                var distance = cluster.distance(point);
                if (distance < minValue) {
                    minValue = distance;
                    minIndex = idx;
                }
            }
            labels[i] = minIndex;
            newClusters[minIndex] = new Cluster(point)

        }

        for cluster in newClusters {
            cluster.calcMean();
        }
        clusters = newClusters;
    }
    writeln(watch.elapsed());

    // Output labels
    var outf = open(outFilename, iomode.cw);
    var writer = outf.writer();
    for lbl in labels {
        writer.writeln(lbl);
    }
    writer.close();
    outf.close();
}
