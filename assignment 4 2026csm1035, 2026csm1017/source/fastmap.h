#ifndef FASTMAP_H
#define FASTMAP_H

#include <vector>
#include <utility>

using namespace std;

extern vector<pair<int, int>> pivots;

bool validateDistanceMatrix(const vector<vector<double>>& distancematrix, int k);

int gettingfarobject(int object, const vector<vector<double>>& distancematrix);

double projectCoordinate(int object, int pivotA, int pivotB, const vector<vector<double>>& distances);

void deflate(vector<vector<double>>& distances, const vector<vector<double>>& coordinates, int dimension);

vector<vector<double>> fastMap(const vector<vector<double>>& distancematrix, int k);

#endif