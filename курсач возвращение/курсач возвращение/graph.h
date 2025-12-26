#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

class Graph {
private:
    int V;
    vector<vector<int>> adjMatrix;
    bool isDirected;
    bool isWeighted;
    string filename;

    void dijkstra(int src, vector<int>& dist, vector<int>& parent);
    vector<int> getPath(int src, int dest, const vector<int>& parent);

public:
    Graph();
    Graph(int vertices, bool directed, bool weighted, const string& fname);

    void generateGraph(int choice, int minWeight = 1, int maxWeight = 10);
    void saveToFile();
    bool loadFromFile(const string& fname);
    static Graph* loadGraphFromFile(const string& fname);
    void printMatrix();
    void printAdjacencyList();
    void runDijkstra();
};

#endif
