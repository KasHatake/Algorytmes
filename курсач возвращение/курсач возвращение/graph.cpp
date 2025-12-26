#include "graph.h"
#include "other.h"
#include <fstream>
#include <random>
#include <iomanip>

void Graph::dijkstra(int src, vector<int>& dist, vector<int>& parent) {
    dist.assign(V, (numeric_limits<int>::max)());
    parent.assign(V, -1);
    dist[src] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, src });

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (int v = 0; v < V; ++v) {
            if (adjMatrix[u][v] != 0) {
                int weight = isWeighted ? adjMatrix[u][v] : 1;
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({ dist[v], v });
                }
            }
        }
    }
}

vector<int> Graph::getPath(int src, int dest, const vector<int>& parent) {
    vector<int> path;
    if (parent[dest] == -1 && dest != src) {
        return path;
    }

    for (int v = dest; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}

Graph::Graph() : V(0), isDirected(false), isWeighted(false) {}

Graph::Graph(int vertices, bool directed, bool weighted, const string& fname)
    : V(vertices), isDirected(directed), isWeighted(weighted), filename(fname) {
    adjMatrix.resize(V, vector<int>(V, 0));
}

void Graph::generateGraph(int choice, int minWeight, int maxWeight) {
    random_device rd;
    mt19937 gen(rd());

    switch (choice) {
    case 1: // Ориентированный невзвешенный
    {
        uniform_int_distribution<> dis(0, 1);
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                adjMatrix[i][j] = dis(gen);
            }
        }
        break;
    }
    case 2: // Ориентированный взвешенный
    {
        uniform_int_distribution<> edgeDis(0, 1);
        uniform_int_distribution<> weightDis(minWeight, maxWeight);
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (edgeDis(gen) == 1) {
                    adjMatrix[i][j] = weightDis(gen);
                }
            }
        }
        break;
    }
    case 3: // Неориентированный невзвешенный
    {
        uniform_int_distribution<> dis(0, 1);
        for (int i = 0; i < V; ++i) {
            for (int j = i; j < V; ++j) {
                int edge = dis(gen);
                adjMatrix[i][j] = edge;
                adjMatrix[j][i] = edge;
            }
        }
        break;
    }
    case 4: // Неориентированный взвешенный
    {
        uniform_int_distribution<> edgeDis(0, 1);
        uniform_int_distribution<> weightDis(minWeight, maxWeight);
        for (int i = 0; i < V; ++i) {
            for (int j = i; j < V; ++j) {
                if (edgeDis(gen) == 1) {
                    int weight = weightDis(gen);
                    adjMatrix[i][j] = weight;
                    adjMatrix[j][i] = weight;
                }
            }
        }
        break;
    }
    }
    saveToFile();
}

void Graph::saveToFile() {
    if (filename.find('.') == string::npos) {
        filename += ".txt";
    }

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка при открытии файла!\n";
        return;
    }

    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            file << adjMatrix[i][j];
            if (j < V - 1) file << " ";
        }
        if (i < V - 1) file << "\n";
    }

    file.close();
    cout << "Граф сохранен в файл: " << filename << "\n";
}

bool Graph::loadFromFile(const string& fname) {
    ifstream file(fname);
    if (!file.is_open()) {
        cerr << "Ошибка при открытии файла!\n";
        return false;
    }

    vector<int> allNumbers;
    int value;
    while (file >> value) {
        allNumbers.push_back(value);
    }

    if (allNumbers.empty()) {
        cerr << "Файл пустой!\n";
        return false;
    }

    V = 0;
    for (int i = 1; i <= 1000; i++) {
        if (i * i == allNumbers.size()) {
            V = i;
            break;
        }
    }

    if (V == 0) {
        cerr << "Некорректный размер матрицы.\n";
        return false;
    }

    isDirected = false;
    isWeighted = false;
    adjMatrix.clear();
    adjMatrix.resize(V, vector<int>(V, 0));
    filename = fname;

    int idx = 0;
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            adjMatrix[i][j] = allNumbers[idx++];
        }
    }

    file.close();
    cout << "Граф загружен из файла: " << fname << "\n";
    return true;
}

Graph* Graph::loadGraphFromFile(const string& fname) {
    Graph* g = new Graph();
    if (g->loadFromFile(fname)) {
        return g;
    }
    else {
        delete g;
        return nullptr;
    }
}

void Graph::printMatrix() {
    cout << "\n=== МАТРИЦА СМЕЖНОСТИ ===\n";

    cout << "    ";
    for (int i = 0; i < V; ++i) cout << setw(4) << i;
    cout << "\n";

    cout << "    ";
    for (int i = 0; i < V; ++i) cout << "----";
    cout << "\n";

    for (int i = 0; i < V; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < V; ++j) {
            cout << setw(4) << adjMatrix[i][j];
        }
        cout << "\n";
    }
}

void Graph::printAdjacencyList() {
    cout << "\n=== СПИСОК СМЕЖНОСТИ ===\n";

    cout << "Вершин: " << V << "\n\n";

    for (int i = 0; i < V; ++i) {
        cout << "Вершина " << i << ": ";

        bool hasNeighbors = false;
        for (int j = 0; j < V; ++j) {
            if (adjMatrix[i][j] != 0) {
                if (hasNeighbors) cout << " -> ";
                cout << j;
                if (isWeighted) cout << "(" << adjMatrix[i][j] << ")";
                hasNeighbors = true;
            }
        }

        if (!hasNeighbors) cout << "нет смежных вершин";
        cout << "\n";
    }
}

void Graph::runDijkstra() {
    cout << "\n=== АЛГОРИТМ ДЕЙКСТРЫ ===\n";

    int startVertex;
    cout << "Введите стартовую вершину (0-" << (V - 1) << "): ";
    cin >> startVertex;

    if (startVertex < 0 || startVertex >= V) {
        cout << "Неверная вершина. Используется вершина 0.\n";
        startVertex = 0;
    }

    cout << "Стартовая вершина: " << startVertex << "\n\n";

    vector<int> dist, parent;
    dijkstra(startVertex, dist, parent);

    cout << "Кратчайшие расстояния от вершины " << startVertex << ":\n";
    for (int i = 0; i < V; ++i) {
        cout << "До вершины " << i << ": ";
        if (dist[i] == (numeric_limits<int>::max)()) {
            cout << "нет пути";
        }
        else {
            cout << dist[i];

            if (i != startVertex && dist[i] != (numeric_limits<int>::max)()) {
                vector<int> path = getPath(startVertex, i, parent);
                if (!path.empty()) {
                    cout << " (путь: ";
                    for (size_t j = 0; j < path.size(); ++j) {
                        cout << path[j];
                        if (j < path.size() - 1) cout << " -> ";
                    }
                    cout << ")";
                }
            }
        }
        cout << "\n";
    }
}