#include "fastmap.h"
#include <cmath>
#include <algorithm>

using namespace std;

vector<pair<int, int>> pivots;

int gettingfarobject(int object, const vector<vector<double>>& distancematrix){
    int N = distancematrix.size();

    int far = object;
    double maxdistance = -1.0;

    for(int i = 0; i < N; i++){
        if(distancematrix[object][i] > maxdistance){
            maxdistance = distancematrix[object][i];
            far = i;
        }
    }

    return far;
}

double projectCoordinate(int object, int pivotA, int pivotB, const vector<vector<double>>& distances){
    double da = distances[object][pivotA];
    double db = distances[object][pivotB];
    double dab = distances[pivotA][pivotB];

    if(dab == 0.0)
        return 0.0;

    double coordinate = (da * da + dab * dab - db * db) / (2.0 * dab);

    return coordinate;
}

void deflate(vector<vector<double>>& distances, const vector<vector<double>>& coordinates, int dimension){
    int N = distances.size();

    for(int a = 0; a < N; a++){
        for(int b = 0; b < N; b++){
            double difference = coordinates[a][dimension] - coordinates[b][dimension];

            double remaining = distances[a][b] * distances[a][b] - difference * difference;

            if(remaining < 0.0)
                remaining = 0.0;

            distances[a][b] = sqrt(remaining);
        }
    }
}

bool validateDistanceMatrix(const vector<vector<double>>& distancematrix, int k){
    int N = distancematrix.size();

    if(N <= 0)
        return false;

    if(k <= 0 || k >= N)
        return false;

    for(int i = 0; i < N; i++){
        if(distancematrix[i].size() != N)
            return false;
    }

    for(int i = 0; i < N; i++){
        if(distancematrix[i][i] != 0.0)
            return false;

        for(int j = 0; j < N; j++){
            if(distancematrix[i][j] < 0.0)
                return false;

            if(distancematrix[i][j] != distancematrix[j][i])
                return false;
        }
    }

    return true;
}

vector<vector<double>> fastMap(const vector<vector<double>>& distancematrix, int k){
    int N = distancematrix.size();

    vector<vector<double>> distances = distancematrix;
    vector<vector<double>> coordinates(N, vector<double>(k, 0.0));

    pivots.clear();

    for(int i = 0; i < k; i++){
        int pivotA = gettingfarobject(0, distances);
        int pivotB = gettingfarobject(pivotA, distances);

        pivots.push_back({pivotA, pivotB});

        for(int object = 0; object < N; object++){
            coordinates[object][i] = projectCoordinate(object, pivotA, pivotB, distances);
        }

        deflate(distances, coordinates, i);
    }

    return coordinates;
}