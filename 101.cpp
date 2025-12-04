#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <random>
#include <iomanip>

using namespace std;

class Graf {
private:
    vector<vector<int>> matrica;
    vector<vector<pair<int, int>>> spiski;
    int vershiny;

    void postroitSpiski() {
        spiski.assign(vershiny, vector<pair<int, int>>());
        for (int i = 0; i < vershiny; ++i) {
            for (int j = 0; j < vershiny; ++j) {
                if (matrica[i][j] > 0) {
                    spiski[i].push_back({ j, matrica[i][j] });
                }
            }
        }
    }

public:
    Graf(int n) : vershiny(n) {
        generirovat();
        postroitSpiski();
    }

    void generirovat() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distVes(1, 10);
        uniform_int_distribution<> estRebro(0, 1);

        matrica.assign(vershiny, vector<int>(vershiny, 0));

        for (int i = 0; i < vershiny; i++) {
            for (int j = i + 1; j < vershiny; j++) {
                if (estRebro(gen)) {
                    int ves = distVes(gen);
                    matrica[i][j] = ves;
                    matrica[j][i] = ves;
                }
            }
        }
    }

    void pechatMatricy() {
        cout << "\nMatrica grafa:\n     ";
        for (int i = 0; i < vershiny; i++) cout << setw(4) << i;
        cout << "\n";
        for (int i = 0; i < vershiny; i++) {
            cout << setw(4) << i << " ";
            for (int j = 0; j < vershiny; j++) cout << setw(4) << matrica[i][j];
            cout << "\n";
        }
    }

    void pechatSpiskov() {
        cout << "\nSpiski smezhnosti:\n";
        for (int i = 0; i < vershiny; i++) {
            cout << i << ": ";
            for (const auto& sosed : spiski[i]) {
                cout << sosed.first << "(" << sosed.second << ") ";
            }
            cout << "\n";
        }
    }

    void naytiPuti(int start) {
        vector<int> dist(vershiny, -1);
        queue<int> q;

        dist[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int tekushchiy = q.front();
            q.pop();

            for (int i = 0; i < vershiny; i++) {
                if (matrica[tekushchiy][i] > 0 && dist[i] == -1) {
                    dist[i] = dist[tekushchiy] + matrica[tekushchiy][i];
                    q.push(i);
                }
            }
        }

        cout << "\nPuti ot vershiny " << start << " (BFS-metod):\n";
        for (int i = 0; i < vershiny; i++) {
            cout << "Do " << i << ": ";
            if (dist[i] == -1) cout << "net puti";
            else cout << dist[i];
            cout << "\n";
        }
    }

    void obhodVglub(int start) {
        cout << "\nObhod v glubinu po spiskam (DFS) ot " << start << ":\n";
        vector<bool> poseschena(vershiny, false);
        stack<int> s;
        s.push(start);

        while (!s.empty()) {
            int tekushchiy = s.top();
            s.pop();

            if (!poseschena[tekushchiy]) {
                cout << tekushchiy << " ";
                poseschena[tekushchiy] = true;
            }

            for (auto it = spiski[tekushchiy].rbegin(); it != spiski[tekushchiy].rend(); ++it) {
                if (!poseschena[it->first]) {
                    s.push(it->first);
                }
            }
        }
        cout << endl;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    int n;
    cout << "Vvedite kolichestvo vershin: ";
    cin >> n;
    if (n <= 0) n = 6;

    Graf graf(n);
    graf.pechatMatricy();
    graf.pechatSpiskov();

    int start;
    cout << "\nVvedite startovuyu vershinu: ";
    cin >> start;

    if (start >= 0 && start < n) {
        graf.naytiPuti(start);
        graf.obhodVglub(start);
    }
    else {
        cout << "Nevernaya vershina!\n";
    }
    return 0;
}
