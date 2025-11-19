#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <iomanip>
#include <chrono>
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
        auto start_time = high_resolution_clock::now();

        vector<int> dist(versh, -1);
        queue<int> ochered;
        dist[start] = 0;
        ochered.push(start);

        while (!ochered.empty()) {
            int curr = ochered.front();
            ochered.pop();
            for (int i = 0; i < versh; i++) {
                if (matrica[curr][i] == 1 && dist[i] == -1) {
                    dist[i] = dist[curr] + 1;
                    ochered.push(i);
                }
            }
        }

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        cout << "Время обхода в ширь (матрица): " << duration.count() << " мкс\n";

        return dist;
    }

    vector<int> bfsSpiski(int start) {
        auto start_time = high_resolution_clock::now();

        vector<int> dist(versh, -1);
        queue<int> ochered;
        dist[start] = 0;
        ochered.push(start);

        while (!ochered.empty()) {
            int curr = ochered.front();
            ochered.pop();
            for (int sosed : spiski[curr]) {
                if (dist[sosed] == -1) {
                    dist[sosed] = dist[curr] + 1;
                    ochered.push(sosed);
                }
            }
        }

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        cout << "Время обхода в ширь (списки): " << duration.count() << " мкс\n";

        return dist;
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
        cout << "Неверное количество вершин! Используется значение по умолчанию: 5\n";
        versh = 5;
    }

    Graf graf(versh);

    graf.pechatMatrica();
    graf.pechatSpiski();

    int start;
    cout << "Введите начальную вершину для обхода в ширину: ";
    cin >> start;
    if (start >= 0 && start < versh) {
        vector<int> dist_m = graf.bfsMatrica(start);
        graf.pechatDist(dist_m, start);

        vector<int> dist_s = graf.bfsSpiski(start);
        graf.pechatDist(dist_s, start);
    }
    else {
        cout << "Неверная вершина!\n";
    }

    return 0;
}

