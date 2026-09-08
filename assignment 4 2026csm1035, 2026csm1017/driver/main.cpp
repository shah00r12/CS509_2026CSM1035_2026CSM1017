#include "kmeans.h"
#include "fastmap.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace chrono;

bool readKMeansFile(const string& filename, int& N, int& D, int& K, int& maxIterations, double& tolerance, vector<vector<double>>& points) {
    ifstream file(filename);
    if(!file)
        return false;

    file >> N >> D >> K;
    if(N <= 0 || D <= 0 || K <= 0 || K > N)
        return false;

    points.resize(N, vector<double>(D));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < D; j++) {
            if(!(file >> points[i][j]))
                return false;
        }
    }

    string keyword;
    file >> keyword;

    if(keyword != "MAX_ITERATIONS")
        return false;

    file >> maxIterations;

    if(maxIterations <= 0)
        return false;

    file >> keyword;

    if(keyword != "TOLERANCE")
        return false;

    file >> tolerance;

    if(tolerance <= 0)
        return false;

    return true;
}

bool readFastMapFile(const string& filename, int& N, int& K, vector<vector<double>>& distances) {
    ifstream file(filename);

    if(!file)
        return false;

    file >> N >> K;

    if(N <= 0 || K <= 0 || K >= N)
        return false;

    distances.resize(N, vector<double>(N));

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(!(file >> distances[i][j]))
                return false;
        }
    }

    for(int i = 0; i < N; i++) {
        if(distances[i][i] != 0.0)
            return false;
        for(int j = 0; j < N; j++) {
            if(distances[i][j] < 0.0)
                return false;

            if(distances[i][j] != distances[j][i])
                return false;
        }
    }

    return true;
}

void runKMeans(const string& filename) {
    int N;
    int D;
    int K;
    int maxIterations;
    double tolerance;

    vector<vector<double>> points;

    if(!readKMeansFile(filename, N, D, K, maxIterations, tolerance, points)) {
        cout << "Invalid K-Means input file." << endl;
        return;
    }

    auto start = high_resolution_clock::now();

    KMeansResult result = kmeans(points, K, maxIterations, tolerance);

    auto stop = high_resolution_clock::now();

    double executionTime = duration<double, milli>(stop - start).count();

    cout << fixed << setprecision(6);

    cout << "Algorithm: K-Means Clustering" << endl;
    cout << "K: " << K << endl;

    cout << "Point assignments:" << endl;

    for(int i = 0; i < N; i++) {
        cout << i << " " << result.assignment[i] << endl;
    }

    cout << "Final centroids:" << endl;
    for(int k = 0; k < K; k++) {
        cout << k << ":";

        for(int j = 0; j < D; j++) {
            cout << " " << result.centroids[k][j];
        }

        cout << endl;
    }

    cout << "WCSS: " << result.wcss << endl;
    cout << "Iterations: " << result.iterations << endl;
    cout << "Converged: " << (result.converged ? "true" : "false") << endl;
    cout << "Execution time: " << executionTime << " ms" << endl;
}

void runFastMap(const string& filename) {
    int N;
    int K;

    vector<vector<double>> distances;

    if(!readFastMapFile(filename, N, K, distances)) {
        cout << "Invalid FastMap input file." << endl;
        return;
    }

    auto start = high_resolution_clock::now();

    vector<vector<double>> coordinates = fastMap(distances, K);

    auto stop = high_resolution_clock::now();

    double executionTime = duration<double, milli>(stop - start).count();

    cout << fixed << setprecision(6);

    cout << "Algorithm: FastMap" << endl;
    cout << "Target dimensions: " << K << endl;

    cout << "Object coordinates:" << endl;
    cout << "Pivots per dimension:" << endl;

    for(int i = 0; i < K; i++){
        cout << "Dim " << i + 1 << ": "
         << pivots[i].first << " "
         << pivots[i].second << endl;
}

    for(int i = 0; i < N; i++) {
        cout << i << ":";

        for(int j = 0; j < K; j++) {
            cout << " " << coordinates[i][j];
        }

        cout << endl;
    }

    cout << "Execution time: " << executionTime << " ms" << endl;
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        cout << "Usage: main <algorithm> <input_file>" << endl;
        cout << "Example: main kmeans testfiles/km_01.txt" << endl;
        cout << "Example: main fastmap testfiles/fm_01.txt" << endl;
        return 1;
    }

    string algorithm = argv[1];
    string filename = argv[2];

    if(algorithm == "kmeans") {
        runKMeans(filename);
    }
    else if(algorithm == "fastmap") {
        runFastMap(filename);
    }
    else {
        cout << "Unknown algorithm." << endl;
        return 1;
    }

    return 0;
}