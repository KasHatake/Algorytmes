#include "graph.h"
#include "other.h"
#include <iostream>

int main() {
    setupConsole();
    cout << "=== ДОБРО ПОЖАЛОВАТЬ В КУРСОВОЙ ПРОЕКТ НА ТЕМУ \"АЛГОРИТМ ДЕЙКСТРЫ\" ===\n\n";
    cout << "=== ГЕНЕРАТОР ГРАФОВ ===\n\n";

    bool running = true;

    while (running) {
        int mainChoice = showMainMenu();
        Graph* g = nullptr;

        switch (mainChoice) {
        case 1: g = createNewGraph(); break;
        case 2: g = loadGraph(); break;
        case 3: running = false; cout << "Выход из программы.\n"; break;
        default: cout << "Неверный выбор.\n"; break;
        }

        if (g != nullptr) {
            g->printMatrix();
            g->printAdjacencyList();
            g->runDijkstra();
            delete g;
        }
        else if (mainChoice == 2) {
            cout << "Не удалось загрузить граф.\n";
            cout << "Нажмите Enter для продолжения...";
            cin.get();
        }
    }
    cout << "\n\n=== БЛАГОДАРИМ ВАС ЗА ИСПОЛЬЗОВАНИЕ ПРОГРАММЫ === \n\n=== ВЫПОЛНЯЕМ ВЫХОД ===\n\n";
    return 0;
}