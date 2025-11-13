#include <iostream> 
#include <vector> 
#include <queue> 
#include <random> 
#include <iomanip> 
#include <string> 
using namespace std;
struct SMEZHLIST {
	int dest;
	SMEZHLIST* next;
	SMEZHLIST(int d) : dest(d), next(nullptr) {}
};
struct AdjList {
	SMEZHLIST* head;
};
class Graph {
private:
	int V;
	vector<vector<int>> adjMatrix;
	vector<AdjList> adjLists;

public:
	Graph(int vertices) : V(vertices) {
		adjMatrix.resize(V, vector<int>(V, 0));
		adjLists.resize(V);
		for (int i = 0; i < V; ++i)
			adjLists[i].head = nullptr;
	}

	~Graph() {
		for (int i = 0; i < V; ++i) {
			SMEZHLIST* curr = adjLists[i].head;
			while (curr) {
				SMEZHLIST* temp = curr;
				curr = curr->next;
				delete temp;
			}
		}
	}
	void genrandgraph() {
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
		spissmezhotmatr();
	}
	void spissmezhotmatr() {
		for (int i = 0; i < V; ++i) {
			SMEZHLIST* curr = adjLists[i].head;
			while (curr) {
				SMEZHLIST* temp = curr;
				curr = curr->next;
				delete temp;
			}
			adjLists[i].head = nullptr;
		}

		for (int i = 0; i < V; ++i) {
			for (int j = 0; j < V; ++j) {
				if (adjMatrix[i][j] == 1)
					rebrovspis(i, j);
			}
		}
	}

	void rebrovspis(int src, int dest) {
		SMEZHLIST* newNode = new SMEZHLIST(dest);
		newNode->next = adjLists[src].head;
		adjLists[src].head = newNode;
	}

	void pringsmezhmatr() {
		cout << "Матрица смежности:\n ";
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

	void pringsmezhspis() {
		cout << "Списки смежности:\n";
		for (int i = 0; i < V; ++i) {
			cout << i << ": ";
			SMEZHLIST* curr = adjLists[i].head;
			while (curr) {
				cout << curr->dest;
				if (curr->next) cout << " -> ";
				curr = curr->next;
			}
			cout << "\n";
		}
		cout << "\n";
	}

	vector<int> BFS_Matrix_StdQueue(int start) {
		vector<int> result;
		vector<bool> visited(V, false);
		queue<int> q;

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

		return result;
	}

	vector<int> BFS_List_StdQueue(int start) {
		vector<int> result;
		vector<bool> visited(V, false);
		queue<int> q;

		visited[start] = true;
		q.push(start);

		while (!q.empty()) {
			int current = q.front();
			q.pop();
			result.push_back(current);

			SMEZHLIST* neighbor = adjLists[current].head;
			while (neighbor) {
				if (!visited[neighbor->dest]) {
					visited[neighbor->dest] = true;
					q.push(neighbor->dest);
				}
				neighbor = neighbor->next;
			}
		}

		return result;
	}

	static void printBFSResult(const vector<int>& result, const string& method) {
		cout << "Результат обхода (" << method << "): ";
		for (size_t i = 0; i < result.size(); ++i) {
			cout << result[i];
			if (i != result.size() - 1) cout << " -> ";
		}
		cout << "\n\n";
	}
};

int safeInputInt(const string& prompt, int minVal, int maxVal) {
	int value;
	while (true) {
		cout << prompt;
		cin >> value;
		if (cin.fail() || value < minVal || value > maxVal) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Ошибка: введите число в диапазоне " << minVal << " - " << maxVal << "!\n";
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return value;
		}
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	int V = safeInputInt("Введите количество вершин графа: ", 1, 10000);

	Graph g(V);
	g.pringsmezhmatr();
	g.genrandgraph();
	g.pringsmezhspis();

	int start = safeInputInt("Введите стартовую вершину (0-" + to_string(V - 1) + "): ", 0, V - 1);


	cout << "BFS для матрицы смежности с std::queue:\n";
	auto res1 = g.BFS_Matrix_StdQueue(start);
	Graph::printBFSResult(res1, "матрица + std::queue");

	cout << "BFS для списков смежности с std::queue:\n";
	auto res2 = g.BFS_List_StdQueue(start);
	Graph::printBFSResult(res2, "списки + std::queue");

	return 0;
}