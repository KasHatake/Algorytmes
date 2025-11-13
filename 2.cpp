#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <random>
#include <iomanip>
using namespace std;
using namespace std::chrono;
class SimpleQueue {
private:
    struct Node {
        int val;
        Node* next;
        Node(int v) : val(v), next(nullptr) {}
    };
    Node* head;
    Node* tail;
public:
    SimpleQueue() : head(nullptr), tail(nullptr) {}
    ~SimpleQueue() { clear(); }
    void push(int val) {
        Node* newNode = new Node(val);
        if (!tail) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    void pop() {
        if (!head) return;
        Node* temp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
    }
    int front() {
        if (head) return head->val;
        return -1;
    }
    bool empty() {
        return head == nullptr;
    }
    void clear() {
        while (!empty()) pop();
    }
};
class Graph {
private:
    int V;
    vector<vector<int>> adjMatrix;
    vector<vector<int>> adjList;

public:
    Graph(int vertices) : V(vertices) {
        adjMatrix.resize(V, vector<int>(V, 0));
        adjList.resize(V);
    }

    void generateRandomGraph() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);
        for (int i = 0; i < V; ++i) {
            for (int j = i + 1; j < V; ++j) {
                int edge = dis(gen);
                adjMatrix[i][j] = edge;
                adjMatrix[j][i] = edge;
            }
        }
        for (int i = 0; i < V; ++i) {
            adjList[i].clear();
            for (int j = 0; j < V; ++j) {
                if (adjMatrix[i][j] == 1) {
                    adjList[i].push_back(j);
                }
            }
        }
    }
    void printAdjMatrix() {
        cout << "Матрица смежности:\n  ";
        for (int i = 0; i < V; ++i)
            cout << setw(3) << i;
        cout << "\n";
        for (int i = 0; i < V; ++i) {
            cout << setw(2) << i;
            for (int j = 0; j < V; ++j) {
                cout << setw(3) << adjMatrix[i][j];
            }
            cout << "\n";
        }
        cout << "\n";
    }
    void printAdjList() {
        cout << "Списки смежности:\n";
        for (int i = 0; i < V; ++i) {
            cout << i << ": ";
            for (auto v : adjList[i]) {
                cout << v << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
    vector<int> BFS_Matrix_StdQueue(int start) {
        vector<int> result;
        vector<bool> visited(V, false);
        std::queue<int> q;
        auto start_time = high_resolution_clock::now();

        visited[start] = true;
        q.push(start);
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            result.push_back(current);

            for (int i = 0; i < V; ++i) {
                if (adjMatrix[current][i] == 1 && !visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);

        cout << "Обход BFS по матрице (std::queue): ";
        for (int v : result) cout << v << "-";
        cout << "\b \n";
        cout << "Время обхода (матрица + std::queue): " << duration.count() << " мкс\n\n";

        return result;
    }
    vector<int> BFS_Matrix_CustomQueue(int start) {
        vector<int> result;
        vector<bool> visited(V, false);
        SimpleQueue sq;
        auto start_time = high_resolution_clock::now();

        visited[start] = true;
        sq.push(start);
        while (!sq.empty()) {
            int current = sq.front();
            sq.pop();
            result.push_back(current);

            for (int i = 0; i < V; ++i) {
                if (adjMatrix[current][i] == 1 && !visited[i]) {
                    visited[i] = true;
                    sq.push(i);
                }
            }
        }

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);

        cout << "Обход BFS по матрице (самописная очередь): ";
        for (int v : result) cout << v << "-";
        cout << "\b \n";
        cout << "Время обхода (матрица + самописная очередь): " << duration.count() << " мкс\n\n";

        return result;
    }
    vector<int> BFS_List_StdQueue(int start) {
        vector<int> result;
        vector<bool> visited(V, false);
        std::queue<int> q;
        auto start_time = high_resolution_clock::now();
        visited[start] = true;
        q.push(start);
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            result.push_back(current);

            for (int v : adjList[current]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        cout << "Обход BFS по списку (std::queue): ";
        for (int v : result) cout << v << "-";
        cout << "\b \n";
        cout << "Время обхода (список + std::queue): " << duration.count() << " мкс\n\n";

        return result;
    }

    vector<int> BFS_List_CustomQueue(int start) {
        vector<int> result;
        vector<bool> visited(V, false);
        SimpleQueue sq;
        auto start_time = high_resolution_clock::now();
        visited[start] = true;
        sq.push(start);
        while (!sq.empty()) {
            int current = sq.front();
            sq.pop();
            result.push_back(current);

            for (int v : adjList[current]) {
                if (!visited[v]) {
                    visited[v] = true;
                    sq.push(v);
                }
            }
        }
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        cout << "Обход BFS по списку (самописная очередь): ";
        for (int v : result) cout << v << "-";
        cout << "\b \n";
        cout << "Время обхода (список + самописная очередь): " << duration.count() << " мкс\n\n";
        return result;
    }
};
int main() {
    setlocale(LC_ALL, "Russian");
    int V;
    cout << "Введите количество вершин графа: ";
    cin >> V;

    Graph g(V);
    g.generateRandomGraph();
    g.printAdjMatrix();
    g.printAdjList();
    int start;
    cout << "Введите стартовую вершину (0-" << V - 1 << "): ";
    cin >> start;
    g.BFS_Matrix_StdQueue(start);
    g.BFS_Matrix_CustomQueue(start);
    g.BFS_List_StdQueue(start);
    g.BFS_List_CustomQueue(start);
    return 0;
}
