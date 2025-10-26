#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    int peak;
    Node** adjLists;
} Graph;

void createMatrixAdj(int** matrix, int peak) {
    for (int i = 0; i < peak; i++) {
        for (int j = i + 1; j < peak; j++) {
            int value = rand() % 2;
            matrix[i][j] = value;
            matrix[j][i] = value;
        }
        matrix[i][i] = 0;
    }
}

void printMatrixAdj(int** matrix, int peak) {
    printf("Матрица смежности (%dx%d):\n", peak, peak);
    printf("   ");
    for (int j = 0; j < peak; j++) {
        printf("%2d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < peak; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < peak; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int** copyMatrixAdj(int** source, int peak) {
    int** buffer = (int**)malloc(peak * sizeof(int*));
    for (int i = 0; i < peak; i++) {
        buffer[i] = (int*)malloc(peak * sizeof(int));
        for (int j = 0; j < peak; j++) {
            buffer[i][j] = source[i][j];
        }
    }
    return buffer;
}

int** identifypeakMatrix(int** matrix, int peak, int p1, int p2, int* newpeak) {
    if (p1 < 1 || p1 > peak || p2 < 1 || p2 > peak || p1 == p2) {
        printf("Ошибка: некорректные номера вершин!\n");
        *newpeak = peak;
        return copyMatrixAdj(matrix, peak);
    }

    int i1 = p1 - 1;
    int i2 = p2 - 1;
    int keepi = (i1 < i2) ? i1 : i2;
    int deli = (i1 < i2) ? i2 : i1;

    *newpeak = peak - 1;

    int** buffer = (int**)malloc(*newpeak * sizeof(int*));
    for (int i = 0; i < *newpeak; i++) {
        buffer[i] = (int*)calloc(*newpeak, sizeof(int));
    }

    for (int i = 0; i < peak; i++) {
        if (i == deli) continue;
        int new_i = (i < deli) ? i : i - 1;
        for (int j = 0; j < peak; j++) {
            if (j == deli) continue;
            int new_j = (j < deli) ? j : j - 1;
            if (i == keepi || j == keepi) {
                if (i == keepi && j == keepi) {
                    buffer[new_i][new_j] = 0;
                }
                else if (i == keepi) {
                    buffer[new_i][new_j] = (matrix[keepi][j] || matrix[deli][j]) ? 1 : 0;
                }
                else if (j == keepi) {
                    buffer[new_i][new_j] = (matrix[i][keepi] || matrix[i][deli]) ? 1 : 0;
                }
            }
            else {
                buffer[new_i][new_j] = matrix[i][j];
            }
        }
    }
    return buffer;
}

int** mergeMatrix(int** matrix, int peak, int p1, int p2, int* newpeak) {
    if (p1 < 1 || p1 > peak || p2 < 1 || p2 > peak || p1 == p2) {
        printf("Ошибка: некорректные номера вершин!\n");
        *newpeak = peak;
        return copyMatrixAdj(matrix, peak);
    }
    int i1 = p1 - 1;
    int i2 = p2 - 1;
    if (matrix[i1][i2] == 0) {
        printf("Ошибка: ребро между вершинами %d и %d не существует!\n", p1, p2);
        *newpeak = peak;
        return copyMatrixAdj(matrix, peak);
    }
    return identifypeakMatrix(matrix, peak, p1, p2, newpeak);
}

int** splitMatrix(int** matrix, int peak, int p, int* newpeak) {
    if (p < 1 || p > peak) {
        printf("Ошибка: некорректный номер вершины!\n");
        *newpeak = peak;
        return copyMatrixAdj(matrix, peak);
    }
    int i = p - 1;
    *newpeak = peak + 1;
    int** buffer = (int**)malloc(*newpeak * sizeof(int*));
    for (int k = 0; k < *newpeak; k++) {
        buffer[k] = (int*)calloc(*newpeak, sizeof(int));
    }
    for (int k = 0; k < peak; k++) {
        for (int j = 0; j < peak; j++) {
            buffer[k][j] = matrix[k][j];
        }
    }
    int newpi = peak;
    buffer[i][newpi] = 1;
    buffer[newpi][i] = 1;
    for (int k = 0; k < *newpeak; k++) {
        buffer[k][k] = 0;
    }
    return buffer;
}

void freemMatrix(int** matrix, int peak) {
    for (int i = 0; i < peak; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int peak) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->peak = peak;
    graph->adjLists = (Node**)malloc(peak * sizeof(Node*));

    for (int i = 0; i < peak; i++) {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

void createMatrixList(Graph* graph, int peak) {
    for (int i = 0; i < peak; i++) {
        for (int j = i + 1; j < peak; j++) {
            if (rand() % 2 == 1) {
                addEdge(graph, i, j);
            }
        }
    }
}

void printMatrixList(Graph* graph) {
    printf("Списки смежности (%d вершин):\n", graph->peak);
    for (int i = 0; i < graph->peak; i++) {
        printf("Вершина %d: ", i + 1);
        Node* temp = graph->adjLists[i];
        if (temp == NULL) {
            printf("нет соседей");
        }
        else {
            while (temp) {
                printf("%d ", temp->vertex + 1);
                temp = temp->next;
            }
        }
        printf("\n");
    }
    printf("\n");
}

Graph* copyMatrixList(Graph* graph) {
    Graph* copy = createGraph(graph->peak);

    for (int i = 0; i < graph->peak; i++) {
        Node* temp = graph->adjLists[i];
        Node** copyPtr = &(copy->adjLists[i]);

        while (temp) {
            *copyPtr = createNode(temp->vertex);
            copyPtr = &((*copyPtr)->next);
            temp = temp->next;
        }
    }

    return copy;
}

void removeVertexFromList(Node** head, int v) {
    Node* temp = *head;
    Node* prev = NULL;

    while (temp != NULL && temp->vertex != v) {
        prev = temp;
        temp = temp->next;
    }

    if (temp != NULL) {
        if (prev == NULL) {
            *head = temp->next;
        }
        else {
            prev->next = temp->next;
        }
        free(temp);
    }
}

int isVertexInList(Node* head, int v) {
    Node* temp = head;
    while (temp != NULL) {
        if (temp->vertex == v) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void freemList(Graph* graph) {
    for (int i = 0; i < graph->peak; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            Node* toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    free(graph->adjLists);
    free(graph);
}

Graph* identifypeakList(Graph* graph, int p1, int p2, int* newpeak) {
    if (p1 < 1 || p1 > graph->peak || p2 < 1 || p2 > graph->peak || p1 == p2) {
        printf("Ошибка: некорректные номера вершин!\n");
        *newpeak = graph->peak;
        return copyMatrixList(graph);
    }

    int i1 = p1 - 1;
    int i2 = p2 - 1;
    int keepi = (i1 < i2) ? i1 : i2;
    int deli = (i1 < i2) ? i2 : i1;

    *newpeak = graph->peak - 1;
    Graph* newGraph = createGraph(*newpeak);

    for (int i = 0; i < graph->peak; i++) {
        if (i == deli) continue;

        int new_i = (i < deli) ? i : i - 1;

        Node* temp = graph->adjLists[i];
        while (temp != NULL) {
            int old_j = temp->vertex;
            int new_j;

            if (old_j == deli || old_j == keepi) {
                new_j = keepi < deli ? keepi : keepi - 1;
            }
            else {
                new_j = (old_j < deli) ? old_j : old_j - 1;
            }

            if (new_j != new_i && !isVertexInList(newGraph->adjLists[new_i], new_j)) {
                Node* newNode = createNode(new_j);
                newNode->next = newGraph->adjLists[new_i];
                newGraph->adjLists[new_i] = newNode;

                Node* newNode2 = createNode(new_i);
                newNode2->next = newGraph->adjLists[new_j];
                newGraph->adjLists[new_j] = newNode2;
            }
            temp = temp->next;
        }
    }

    return newGraph;
}

Graph* mergeList(Graph* graph, int p1, int p2, int* newpeak) {
    if (p1 < 1 || p1 > graph->peak || p2 < 1 || p2 > graph->peak || p1 == p2) {
        printf("Ошибка: некорректные номера вершин!\n");
        *newpeak = graph->peak;
        return copyMatrixList(graph);
    }

    int i1 = p1 - 1;
    int i2 = p2 - 1;

    if (!isVertexInList(graph->adjLists[i1], i2)) {
        printf("Ошибка: ребро между вершинами %d и %d не существует!\n", p1, p2);
        *newpeak = graph->peak;
        return copyMatrixList(graph);
    }

    return identifypeakList(graph, p1, p2, newpeak);
}

Graph* splitList(Graph* graph, int p, int* newpeak) {
    if (p < 1 || p > graph->peak) {
        printf("Ошибка: некорректный номер вершины!\n");
        *newpeak = graph->peak;
        return copyMatrixList(graph);
    }

    int i = p - 1;
    *newpeak = graph->peak + 1;
    Graph* newGraph = createGraph(*newpeak);
    int newpi = graph->peak;

    for (int k = 0; k < graph->peak; k++) {
        Node* temp = graph->adjLists[k];
        while (temp != NULL) {
            int old_j = temp->vertex;
            if (!isVertexInList(newGraph->adjLists[k], old_j) && k != old_j) {
                Node* newNode = createNode(old_j);
                newNode->next = newGraph->adjLists[k];
                newGraph->adjLists[k] = newNode;
            }
            temp = temp->next;
        }
    }

    if (!isVertexInList(newGraph->adjLists[i], newpi)) {
        Node* newNode1 = createNode(newpi);
        newNode1->next = newGraph->adjLists[i];
        newGraph->adjLists[i] = newNode1;

        Node* newNode2 = createNode(i);
        newNode2->next = newGraph->adjLists[newpi];
        newGraph->adjLists[newpi] = newNode2;
    }

    return newGraph;
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));

    int representationType;
    printf("Выберите представление графа:\n");
    printf("1 - Матрица смежности\n");
    printf("2 - Списки смежности\n");
    printf("Выбор: ");
    scanf("%d", &representationType);

    if (representationType != 1 && representationType != 2) {
        printf("Ошибка: некорректный выбор!\n");
        return 1;
    }

    int peak;
    printf("Введите количество вершин для графа: ");
    scanf("%d", &peak);

    if (peak <= 0) {
        printf("Ошибка: количество вершин должно быть положительным числом.\n");
        return 1;
    }

    if (representationType == 1) {
        int** matrix = (int**)malloc(peak * sizeof(int*));
        for (int i = 0; i < peak; i++) {
            matrix[i] = (int*)malloc(peak * sizeof(int));
        }

        createMatrixAdj(matrix, peak);
        printf("\nИсходный граф\n");
        printMatrixAdj(matrix, peak);

        int choice;
        int p1, p2, p;
        int newpeak;
        do {
            printf("\nОперации над графом \n");
            printf("1 - Отождествление вершин\n");
            printf("2 - Стягивание ребра\n");
            printf("3 - Расщепление вершины\n");
            printf("0 - Выход\n");
            printf("Выберите операцию: ");
            scanf("%d", &choice);
            switch (choice) {
            case 1: {
                printf("Введите номера вершин для отождествления (через пробел): ");
                scanf("%d %d", &p1, &p2);
                int** result = identifypeakMatrix(matrix, peak, p1, p2, &newpeak);
                printf("\nРезультат отождествления вершин %d и %d:\n", p1, p2);
                printMatrixAdj(result, newpeak);
                freemMatrix(result, newpeak);
                break;
            }
            case 2: {
                printf("Введите номера вершин ребра для стягивания (через пробел): ");
                scanf("%d %d", &p1, &p2);
                int** result = mergeMatrix(matrix, peak, p1, p2, &newpeak);
                printf("\nРезультат стягивания ребра (%d, %d):\n", p1, p2);
                printMatrixAdj(result, newpeak);
                freemMatrix(result, newpeak);
                break;
            }
            case 3: {
                printf("Введите номер вершины для расщепления: ");
                scanf("%d", &p);
                int** result = splitMatrix(matrix, peak, p, &newpeak);
                printf("\nРезультат расщепления вершины %d:\n", p);
                printMatrixAdj(result, newpeak);
                freemMatrix(result, newpeak);
                break;
            }
            case 0:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Неверный выбор!\n");
            }
        } while (choice != 0);
        freemMatrix(matrix, peak);
    }
    else if (representationType == 2) {
        Graph* graph = createGraph(peak);
        createMatrixList(graph, peak);
        printf("\nИсходный граф\n");
        printMatrixList(graph);

        int choice;
        int p1, p2, p;
        int newpeak;

        do {
            printf("\nОперации над графом\n");
            printf("1 - Отождествление вершин\n");
            printf("2 - Стягивание ребра\n");
            printf("3 - Расщепление вершины\n");
            printf("0 - Выход\n");
            printf("Выберите операцию: ");
            scanf("%d", &choice);

            Graph* result = NULL;

            switch (choice) {
            case 1: {
                printf("Введите номера вершин для отождествления (через пробел): ");
                scanf("%d %d", &p1, &p2);
                result = identifypeakList(graph, p1, p2, &newpeak);
                printf("\nРезультат отождествления вершин %d и %d:\n", p1, p2);
                printMatrixList(result);
                freemList(result);
                break;
            }
            case 2: {
                printf("Введите номера вершин ребра для стягивания (через пробел): ");
                scanf("%d %d", &p1, &p2);
                result = mergeList(graph, p1, p2, &newpeak);
                printf("\nРезультат стягивания ребра (%d, %d):\n", p1, p2);
                printMatrixList(result);
                freemList(result);
                break;
            }
            case 3: {
                printf("Введите номер вершины для расщепления: ");
                scanf("%d", &p);
                result = splitList(graph, p, &newpeak);
                printf("\nРезультат расщепления вершины %d:\n", p);
                printMatrixList(result);
                freemList(result);
                break;
            }
            case 0:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Неверный выбор!\n");
            }
        } while (choice != 0);

        freemList(graph);
    }

    return 0;
}
