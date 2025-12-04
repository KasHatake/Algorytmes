#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <string>

using namespace std;

class Graf {
private:
    vector<vector<int>> matrica;
    int vershiny;
    bool orientir;

    vector<int> naytiPuti(int start) {
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
        return dist;
    }

public:
    Graf(int n, bool isOriented) : vershiny(n), orientir(isOriented) {
        generirovat();
    }

    void generirovat() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distVes(1, 10);
        uniform_int_distribution<> estRebro(0, 2);

        matrica.assign(vershiny, vector<int>(vershiny, 0));

        for (int i = 0; i < vershiny; i++) {
            for (int j = 0; j < vershiny; j++) {
                if (i == j) continue;
                if (orientir) {
                    if (estRebro(gen) > 0) matrica[i][j] = distVes(gen);
                }
                else if (i < j && estRebro(gen) > 0) {
                    int ves = distVes(gen);
                    matrica[i][j] = matrica[j][i] = ves;
                }
            }
        }
    }

    void pechatMatricy() {
        cout << "\nMatrica " << (orientir ? "orientirovannogo" : "neorientirovannogo") << " grafa:\n     ";
        for (int i = 0; i < vershiny; i++) cout << setw(4) << i;
        cout << "\n";
        for (int i = 0; i < vershiny; i++) {
            cout << setw(4) << i << " ";
            for (int j = 0; j < vershiny; j++) cout << setw(4) << matrica[i][j];
            cout << "\n";
        }
    }

    int getEkstsentrisitet(int v) {
        vector<int> dist = naytiPuti(v);
        int maxDist = 0;
        for (int d : dist) {
            if (d == -1) return numeric_limits<int>::max();
            maxDist = max(maxDist, d);
        }
        return maxDist;
    }

    void analiz() {
        vector<int> ekstsentrisitety(vershiny);
        int radius = numeric_limits<int>::max();
        int diametr = 0;

        for (int i = 0; i < vershiny; i++) {
            ekstsentrisitety[i] = getEkstsentrisitet(i);
            if (ekstsentrisitety[i] != numeric_limits<int>::max()) {
                radius = min(radius, ekstsentrisitety[i]);
                diametr = max(diametr, ekstsentrisitety[i]);
            }
        }

        cout << "\nAnaliz grafa\n";
        cout << "Radius: " << (radius == numeric_limits<int>::max() ? "ne opredelen" : to_string(radius)) << endl;
        cout << "Diametr: " << (diametr == 0 ? "ne opredelen" : to_string(diametr)) << endl;

        cout << "Tsentralnye vershiny: ";
        for (int i = 0; i < vershiny; i++) if (ekstsentrisitety[i] == radius) cout << i << " ";
        cout << endl;

        cout << "Periferiynye vershiny: ";
        for (int i = 0; i < vershiny; i++) if (ekstsentrisitety[i] == diametr) cout << i << " ";
        cout << endl;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    int n, tip;
    cout << "Vvedite kolichestvo vershin: ";
    cin >> n;
    if (n <= 0) n = 6;

    cout << "\nVyberite tip grafa (1 - neorient., 2 - orient.): ";
    cin >> tip;

    Graf graf(n, tip == 2);
    graf.pechatMatricy();
    graf.analiz();

    return 0;
}
