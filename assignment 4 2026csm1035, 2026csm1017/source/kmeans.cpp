#include "kmeans.h"

#include <cmath>
#include <limits>

using namespace std;

KMeansResult kmeans(const vector<vector<double>>& points,
    int K,
    int maxIterations,
    double tolerance
)
{
    KMeansResult result;
    int N = points.size();
    if(N == 0 || K <= 0 || K > N)
        return result;

    int D = points[0].size();
    if(D == 0 || maxIterations <= 0 || tolerance < 0)
        return result;
    for(int i = 0; i < N; i++){
        if(points[i].size() != D)
            return result;
    }

    vector<vector<double>> centroids( K, vector<double>(D));
    for(int k = 0; k < K; k++){
        centroids[k] = points[k];
    }

    vector<int> assignment(N, -1);

    bool converged = false;
    int iterations = 0;

    for(int iteration = 0; iteration < maxIterations; iteration++){
        for(int i = 0; i < N; i++){
            double bestDistance = numeric_limits<double>::max();
            int bestCluster = -1;
            for(int k = 0; k < K; k++){
                double distance = 0.0;
                for(int j = 0; j < D; j++){
                    double difference = points[i][j] - centroids[k][j];
                    distance += difference * difference;
                }

                if(distance < bestDistance){
                    bestDistance = distance;
                    bestCluster = k;
                }
            }

            assignment[i] = bestCluster;
        }
        vector<vector<double>> newCentroids(K, vector<double>(D, 0.0));
        vector<int> count(K, 0);
        for(int i = 0; i < N; i++){
            int cluster = assignment[i];
            count[cluster]++;
            for(int j = 0; j < D; j++){
                newCentroids[cluster][j] += points[i][j];
            }
        }
        for(int k = 0; k < K; k++){
            if(count[k] > 0){
                for(int j = 0; j < D; j++) {
                    newCentroids[k][j] /= count[k];
                }
            }
            else{
                newCentroids[k] = centroids[k];
            }
        }
        double centroidChange = 0.0;

        for(int k = 0; k < K; k++){
            for(int j = 0; j < D; j++){
                double difference = newCentroids[k][j] - centroids[k][j];
                centroidChange += difference * difference;
            }
        }

        centroids = newCentroids;
        iterations = iteration + 1;

        if(centroidChange <= tolerance){
            converged = true;
            break;
        }
    }
    double wcss = 0.0;
    for(int i = 0; i < N; i++){
        int cluster = assignment[i];
        double distance = 0.0;
        for(int j = 0; j < D; j++){
            double difference = points[i][j] - centroids[cluster][j];

            distance += difference * difference;
        }

        wcss += distance;
    }
    result.assignment = assignment;
    result.centroids = centroids;
    result.wcss = wcss;
    result.iterations = iterations;
    result.converged = converged;

    return result;
}