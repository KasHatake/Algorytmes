#include "other.h"
#include "graph.h"
#include <fstream>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

void setupConsole() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
}

string cleanFilename(const string& input) {
    string result;
    for (unsigned char c : input) {
        if ((c >= 192 && c <= 255) || c == 168 || c == 184) {
            result += c;
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9')) {
            result += c;
        }
        else if (c == '_' || c == '-' || c == '.' || c == ' ') {
            result += c;
        }
        else {
            result += '_';
        }
    }

    while (!result.empty() && result.front() == ' ') {
        result.erase(0, 1);
    }
    while (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }

    if (result.empty()) {
        result = "graph";
    }

    return result;
}

int inputInt(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < minVal || value > maxVal) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "Ошибка: введите число от " << minVal << " до " << maxVal << "!\n";
        }
        else {
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            return value;
        }
    }
}

int showMainMenu() {
    cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
    cout << "1. Создать новый граф\n";
    cout << "2. Загрузить граф из файла\n";
    cout << "3. Выйти из программы\n";
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;
    cin.ignore();
    return choice;
}

Graph* createNewGraph() {
    cout << "\nВыберите тип графа:\n";
    cout << "1. Ориентированный невзвешенный\n";
    cout << "2. Ориентированный взвешенный\n";
    cout << "3. Неориентированный невзвешенный\n";
    cout << "4. Неориентированный взвешенный\n";
    cout << "5. Вернуться в меню\n";
    cout << "Ваш выбор: ";

    int choice = inputInt("", 1, 5);
    if (choice == 5) return nullptr;

    string filename;
    cout << "\nВведите имя файла: ";
    getline(cin, filename);

    filename = cleanFilename(filename);
    if (filename.empty()) filename = "graph";
    if (filename.find('.') == string::npos) filename += ".txt";

    cout << "Имя файла: " << filename << "\n";

    int V = inputInt("Введите количество вершин: ", 1, 100);

    ifstream testFile(filename);
    if (testFile.good()) {
        testFile.close();
        cout << "\nФайл уже существует.\n";
        cout << "1. Перезаписать\n2. Вернуться\nВаш выбор: ";

        int loadChoice = inputInt("", 1, 2);
        if (loadChoice == 2) return nullptr;
        cout << "Файл будет перезаписан.\n";
    }

    Graph* g = new Graph(V, choice == 1 || choice == 2, choice == 2 || choice == 4, filename);

    if (choice == 2 || choice == 4) {
        int minW = inputInt("Минимальный вес: ", 1, 100);
        int maxW = inputInt("Максимальный вес: ", minW, 1000);
        g->generateGraph(choice, minW, maxW);
    }
    else {
        g->generateGraph(choice);
    }

    return g;
}

Graph* loadGraph() {
    string filename;
    cout << "Введите имя файла: ";
    getline(cin, filename);
    return Graph::loadGraphFromFile(filename);
}