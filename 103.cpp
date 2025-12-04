#include <iostream>
#include <vector> // 
#include <queue>
#include <random>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <string>
#include <cstring>

using namespace std;

class Graf {
private:
    vector<vector<int>> matrica;
    int vershiny;
    bool orientir;

    vector<int> naytiPuti(int start) {
        if (start < 0 || start >= vershiny) {
            return vector<int>(vershiny, -1);
        }
        vector<int> dist(vershiny, -1);
        queue<int> q;
        dist[start] = 0;
        q.push(start);

        while (!q.empty()) {
            int tek = q.front();
            q.pop();
            for (int i = 0; i < vershiny; i++) {
                if (matrica[tek][i] > 0 && dist[i] == -1) {
                    dist[i] = dist[tek] + matrica[tek][i];
                    q.push(i);
                }
            }
        }
        return dist;
    }

public:
    Graf(int n, bool isOriented = false) : vershiny(n), orientir(isOriented) {
        generirovat();
    }

    void generirovat() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distVes(1, 10);
        uniform_int_distribution<> distRebro(0, 2);

        matrica.assign(vershiny, vector<int>(vershiny, 0));

        for (int i = 0; i < vershiny; i++) {
            for (int j = 0; j < vershiny; j++) {
                if (i == j) continue;
                if (orientir) {
                    if (distRebro(gen) > 0) matrica[i][j] = distVes(gen);
                }
                else if (i < j && distRebro(gen) > 0) {
                    int ves = distVes(gen);
                    matrica[i][j] = matrica[j][i] = ves;
                }
            }
        }
    }

    void pechatMatricy() {
        cout << "\nMatrica " << (orientir ? "orientirovannogo" : "neorientirovannogo") << " grafa:\n";
        cout << "   ";
        for (int i = 0; i < vershiny; i++) cout << setw(4) << i;
        cout << "\n";
        for (int i = 0; i < vershiny; i++) {
            cout << setw(3) << i;
            for (int j = 0; j < vershiny; j++) cout << setw(4) << matrica[i][j];
            cout << "\n";
        }
    }

    vector<vector<int>> naytiMatricuRasstoyaniy() {
        const int INF = numeric_limits<int>::max() / 2;
        int n = vershiny;
        vector<vector<int>> dist(n, vector<int>(n, INF));
        for (int i = 0; i < n; i++) {
            dist[i][i] = 0;
            for (int j = 0; j < n; j++) {
                if (matrica[i][j] > 0) dist[i][j] = matrica[i][j];
            }
        }
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
        return dist;
    }

    void pechatMatricyRasstoyaniy() {
        cout << "\nMatrica rasstoyaniy:\n   ";
        auto dist = naytiMatricuRasstoyaniy();
        for (int i = 0; i < vershiny; i++) cout << setw(6) << i;
        cout << "\n";
        for (int i = 0; i < vershiny; i++) {
            cout << setw(3) << i;
            for (int j = 0; j < vershiny; j++) {
                if (dist[i][j] == numeric_limits<int>::max() / 2)
                    cout << setw(6) << "inf";
                else
                    cout << setw(6) << dist[i][j];
            }
            cout << "\n";
        }
    }

    void vyvestiPuti(int start) {
        vector<int> dist = naytiPuti(start);
        cout << "Puti ot vershiny " << start << ":\n";
        for (int i = 0; i < vershiny; i++) {
            cout << "Do " << i << ": ";
            if (dist[i] == -1) cout << "net puti";
            else cout << dist[i];
            cout << "\n";
        }
    }

    int getEksc(int v) {
        if (v < 0 || v >= vershiny) return -1;
        vector<int> dist = naytiPuti(v);
        int maxDist = 0;
        for (int d : dist) {
            if (d == -1) return numeric_limits<int>::max();
            maxDist = max(maxDist, d);
        }
        return maxDist;
    }

    void analiz() {
        auto dist = naytiMatricuRasstoyaniy();
        vector<int> ekscVseh(vershiny);
        int radius = numeric_limits<int>::max();
        int diametr = 0;


        for (int i = 0; i < vershiny; i++) {
            int maxDist = 0;
            bool hasPath = false;
            for (int j = 0; j < vershiny; j++) {
                if (dist[i][j] == numeric_limits<int>::max() / 2)
                    continue;
                if (dist[i][j] > maxDist)
                    maxDist = dist[i][j];
            }
            if (!hasPath)
                ekscVseh[i] = numeric_limits<int>::max();
            else
                ekscVseh[i] = maxDist;
        }

        vector<int> tsentry, periferiya;
        for (int i = 0; i < vershiny; i++) {
            if (ekscVseh[i] == radius) tsentry.push_back(i);
            if (ekscVseh[i] == diametr) periferiya.push_back(i);
        }

        cout << "\nЭкцентриситеты:\n";
        for (int i = 0; i < vershiny; i++) {
            cout << "Вершина " << i << ": "
                << (ekscVseh[i] == numeric_limits<int>::max() ? "inf" : to_string(ekscVseh[i])) << "\n";
        }
        cout << "\nРадиус графа: " << (radius == numeric_limits<int>::max() ? "не определен" : to_string(radius)) << endl;
        cout << "Диаметр графа: " << (diametr == 0 ? "не определен" : to_string(diametr)) << endl;
        cout << "Центральные вершины: ";
        for (int v : tsentry) cout << v << " ";
        cout << endl;
        cout << "Периферийные вершины: ";
        for (int v : periferiya) cout << v << " ";
        cout << endl;
    }


    int getVershiny() const { return vershiny; }
    bool isOrientir() const { return orientir; }
    void setVershiny(int n) { vershiny = n; generirovat(); }
    void setOrientir(bool isOriented) { orientir = isOriented; generirovat(); }
};

void obrabotatKomandy(int argc, char* argv[], int& vershiny, bool& orientir, bool& delatAnaliz, int& start, bool& interaktiv, bool& printDistanceMatrix) {
    vershiny = 6;
    orientir = false;
    delatAnaliz = false;
    start = -1;
    interaktiv = (argc == 1);
    printDistanceMatrix = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) vershiny = atoi(argv[++i]);
        else if (strcmp(argv[i], "-d") == 0) orientir = true;
        else if (strcmp(argv[i], "-u") == 0) orientir = false;
        else if (strcmp(argv[i], "-a") == 0) delatAnaliz = true;
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) start = atoi(argv[++i]);
        else if (strcmp(argv[i], "-r") == 0) printDistanceMatrix = true;
    }

    if (delatAnaliz || start != -1) interaktiv = false;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    int vershiny, start, novyeVershiny, vybor;
    bool orientir, delatAnaliz, interaktiv, printDistanceMatrix;

    obrabotatKomandy(argc, argv, vershiny, orientir, delatAnaliz, start, interaktiv, printDistanceMatrix);

    if (vershiny <= 0) vershiny = 6;

    Graf graf(vershiny, orientir);

    if (!interaktiv) {
        graf.pechatMatricy();
        if (printDistanceMatrix) graf.pechatMatricyRasstoyaniy();
        if (delatAnaliz) graf.analiz();
        if (start != -1) {
            if (start >= 0 && start < graf.getVershiny()) graf.vyvestiPuti(start);
            else cout << "Nevernaya startovaya vershina: " << start << endl;
        }
        return 0;
    }

    do {
        cout << "\n1. Neorientirovannyy graf";
        cout << "\n2. Orientirovannyy graf";
        cout << "\n3. Analiz grafa";
        cout << "\n4. Izmenit' kolichestvo vershin";
        cout << "\n0. Vyhod";
        cout << "\nVyberite operatsiyu: ";
        cin >> vybor;
        switch (vybor) {
        case 1:
            if (graf.isOrientir()) graf.setOrientir(false);
            graf.pechatMatricy();
            cout << "Vvedite startovuyu vershinu: ";
            cin >> start;
            if (start >= 0 && start < graf.getVershiny()) graf.vyvestiPuti(start);
            else cout << "Nevernaya vershina!\n";
            break;
        case 2:
            if (!graf.isOrientir()) graf.setOrientir(true);
            graf.pechatMatricy();
            cout << "Vvedite startovuyu vershinu: ";
            cin >> start;
            if (start >= 0 && start < graf.getVershiny()) graf.vyvestiPuti(start);
            else cout << "Nevernaya vershina!\n";
            break;
        case 3:
            graf.pechatMatricy();
            graf.analiz();
            break;
        case 4:
            cout << "Vvedite novoe kolichestvo vershin: ";
            cin >> novyeVershiny;
            if (novyeVershiny > 0) {
                graf.setVershiny(novyeVershiny);
                graf.pechatMatricy();
            }
            else cout << "Nevernoe kolichestvo!\n";
            break;
        }
    } while (vybor != 0);
    return 0;
}
