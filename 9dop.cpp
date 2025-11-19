#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <random>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

class Graf {
private:
    vector<vector<int>> matrica;
    vector<vector<int>> spiski;
    int versh;

    void postroenieSpiskov() {
        spiski.clear();
        spiski.resize(versh);
        for (int i = 0; i < versh; i++) {
            for (int j = 0; j < versh; j++) {
                if (matrica[i][j] == 1) {
                    spiski[i].push_back(j);
                }
            }
        }
    }

public:
    Graf(int n) : versh(n) {
        generaciaGraf();
    }

    void generaciaGraf() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 1);
        matrica.resize(versh, vector<int>(versh, 0));
        for (int i = 0; i < versh; i++) {
            for (int j = i + 1; j < versh; j++) {
                int rebro = dis(gen);
                matrica[i][j] = rebro;
                matrica[j][i] = rebro;
            }
        }
        postroenieSpiskov();
    }

    void pechatMatrica() {
        cout << "\nМатрица смежности:\n   ";
        for (int i = 0; i < versh; i++)
            cout << setw(3) << i;
        cout << "\n";
        for (int i = 0; i < versh; i++) {
            cout << setw(3) << i;
            for (int j = 0; j < versh; j++)
                cout << setw(3) << matrica[i][j];
            cout << "\n";
        }
    }

    void pechatSpiski() {
        cout << "\nСписки смежности:\n";
        for (int i = 0; i < versh; i++) {
            cout << i << ": ";
            for (int sosed : spiski[i]) {
                cout << sosed << " ";
            }
            cout << "\n";
        }
    }

    vector<int> bfsMatrica(int start) {
        vector<int> dist(versh, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            for (int i = 0; i < versh; i++) {
                if (matrica[curr][i] == 1 && dist[i] == -1) {
                    dist[i] = dist[curr] + 1;
                    q.push(i);
                }
            }
        }
        return dist;
    }

    vector<int> bfsSpiski(int start) {
        vector<int> dist(versh, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            for (int sosed : spiski[curr]) {
                if (dist[sosed] == -1) {
                    dist[sosed] = dist[curr] + 1;
                    q.push(sosed);
                }
            }
        }
        return dist;
    }

    vector<int> dfsMatrica(int start) {
        vector<int> dist(versh, -1);
        stack<int> s;
        dist[start] = 0;
        s.push(start);
        while (!s.empty()) {
            int curr = s.top();
            s.pop();
            for (int i = 0; i < versh; i++) {
                if (matrica[curr][i] == 1 && dist[i] == -1) {
                    dist[i] = dist[curr] + 1;
                    s.push(i);
                }
            }
        }
        return dist;
    }

    vector<int> dfsSpiski(int start) {
        vector<int> dist(versh, -1);
        stack<int> s;
        dist[start] = 0;
        s.push(start);
        while (!s.empty()) {
            int curr = s.top();
            s.pop();
            for (int sosed : spiski[curr]) {
                if (dist[sosed] == -1) {
                    dist[sosed] = dist[curr] + 1;
                    s.push(sosed);
                }
            }
        }
        return dist;
    }

    vector<vector<int>> postroitMatricuRasstoyaniyBFSMatrica() {
        auto time_start = high_resolution_clock::now();

        vector<vector<int>> matricaRasstoyaniy(versh, vector<int>(versh, -1));

        for (int i = 0; i < versh; i++) {
            matricaRasstoyaniy[i] = bfsMatrica(i);
        }


        auto time_end = high_resolution_clock::now();
        cout << "Время построения матрицы расстояний (BFS + матрица): "
            << duration_cast<microseconds>(time_end - time_start).count() << " мкс\n";

        return matricaRasstoyaniy;
    }

    vector<vector<int>> postroitMatricuRasstoyaniyDFSMatrica() {
        auto time_start = high_resolution_clock::now();

        vector<vector<int>> matricaRasstoyaniy(versh, vector<int>(versh, -1));

        for (int i = 0; i < versh; i++) {
            matricaRasstoyaniy[i] = dfsMatrica(i);
        }

        auto time_end = high_resolution_clock::now();
        cout << "Время построения матрицы расстояний (DFS + матрица): "
            << duration_cast<microseconds>(time_end - time_start).count() << " мкс\n";

        return matricaRasstoyaniy;
    }

    void pechatMatricaRasstoyaniy(const vector<vector<int>>& matricaRasst) {
        cout << "\nМатрица расстояний:\n   ";
        for (int i = 0; i < versh; i++)
            cout << setw(3) << i;
        cout << "\n";
        for (int i = 0; i < versh; i++) {
            cout << setw(3) << i;
            for (int j = 0; j < versh; j++) {
                if (matricaRasst[i][j] == -1)
                    cout << setw(3) << "-";
                else
                    cout << setw(3) << matricaRasst[i][j];
            }
            cout << "\n";
        }
    }

    void pechatDist(const vector<int>& dist, int start) {
        cout << "\nРасстояния от вершины " << start << ":\n";
        for (int i = 0; i < versh; i++) {
            cout << "Вершина " << i << ": ";
            if (dist[i] == -1)
                cout << "-";
            else
                cout << dist[i];
            cout << "\n";
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    int versh;
    cout << "Введите количество вершин графа: ";
    cin >> versh;

    if (versh <= 0) {
        cout << "Неверное количество вершин! Устанавливается значение по умолчанию: 5\n";
        versh = 5;
    }

    Graf graf(versh);

    graf.pechatMatrica();
    graf.pechatSpiski();

    auto matricaRasst1 = graf.postroitMatricuRasstoyaniyBFSMatrica();
    graf.pechatMatricaRasstoyaniy(matricaRasst1);

    auto matricaRasst3 = graf.postroitMatricuRasstoyaniyDFSMatrica();
    graf.pechatMatricaRasstoyaniy(matricaRasst3);

    int start;
    cout << "\nВведите стартовую вершину для обхода конкретной вершины: ";
    cin >> start;

    if (start >= 0 && start < versh) {
        auto dist_bfs_mat = graf.bfsMatrica(start);
        graf.pechatDist(dist_bfs_mat, start);
        cout << "Расстояние BFS матрица \n";

        auto dist_bfs_spis = graf.bfsSpiski(start);
        graf.pechatDist(dist_bfs_spis, start);
        cout << "Рассстояние BFS списки \n";

        auto dist_dfs_mat = graf.dfsMatrica(start);
        graf.pechatDist(dist_dfs_mat, start);
        cout << "Расстояние DFS матрица \n";

        auto dist_dfs_spis = graf.dfsSpiski(start);
        graf.pechatDist(dist_dfs_spis, start);
        cout << "Расстояние DFS списки \n";
    }
    else {
        cout << "Неверная вершина!\n";
    }

    return 0;
}