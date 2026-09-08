#ifndef KMEANS_H
#define KMEANS_H

#include <vector>

using namespace std;
struct KMeansResult{
    vector<int> assignment;
    vector<vector<double>> centroids;
    double wcss;
    int iterations;
    bool converged;
};

KMeansResult kmeans(
    const vector<vector<double>>& points,
    int K,
    int maxIterations,
    double tolerance
);

#endif