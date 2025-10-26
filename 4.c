#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

void createMatrix(int** matrix, int peak) {
    for (int i = 0; i < peak; i++) {
        for (int j = i + 1; j < peak; j++) {
            int value = rand() % 2;
            matrix[i][j] = value;
            matrix[j][i] = value;
        }
        matrix[i][i] = 0;
    }
}

void printMatrix(int** matrix, int peak, const char* name) {
    printf("%s (%dx%d):\n", name, peak, peak);
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

int** createMatrixFunc(int peak) {
    int** matrix = (int**)malloc(peak * sizeof(int*));
    for (int i = 0; i < peak; i++) {
        matrix[i] = (int*)calloc(peak, sizeof(int));
    }
    return matrix;
}

void freem(int** matrix, int peak) {
    for (int i = 0; i < peak; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int** cartesianProduct(int** matrix1, int peak1, int** matrix2, int peak2, int* resultPeak) {
    *resultPeak = peak1 * peak2;
    int** result = createMatrixFunc(*resultPeak);

    for (int u1 = 0; u1 < peak1; u1++) {
        for (int v1 = 0; v1 < peak2; v1++) {
            for (int u2 = 0; u2 < peak1; u2++) {
                for (int v2 = 0; v2 < peak2; v2++) {
                    int i = u1 * peak2 + v1;
                    int j = u2 * peak2 + v2;
                    if ((u1 == u2 && matrix2[v1][v2] == 1) || (v1 == v2 && matrix1[u1][u2] == 1)) {
                        result[i][j] = 1;
                    }
                }
            }
        }
    }
    return result;
}

void printCartesianMatrix(int** matrix, int peak1, int peak2, const char* name) {
    int totalPeak = peak1 * peak2;
    printf("%s (%dx%d):\n", name, totalPeak, totalPeak);
    printf("    ");
    for (int u = 0; u < peak1; u++) {
        for (int v = 0; v < peak2; v++) {
            printf("(%d,%d) ", u + 1, v + 1);
        }
    }
    printf("\n");

    for (int u1 = 0; u1 < peak1; u1++) {
        for (int v1 = 0; v1 < peak2; v1++) {
            int i = u1 * peak2 + v1;
            printf("(%d,%d)", u1 + 1, v1 + 1);
            for (int u2 = 0; u2 < peak1; u2++) {
                for (int v2 = 0; v2 < peak2; v2++) {
                    int j = u2 * peak2 + v2;
                    printf("%2d    ", matrix[i][j]);
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

void printCartesianAdjacencyList(int** matrix, int peak1, int peak2) {
    int totalPeak = peak1 * peak2;
    printf("Списки смежности декартова произведения:\n");
    for (int u1 = 0; u1 < peak1; u1++) {
        for (int v1 = 0; v1 < peak2; v1++) {
            int i = u1 * peak2 + v1;
            printf("Вершина (%d,%d): ", u1 + 1, v1 + 1);
            int hasNeighbors = 0;
            for (int u2 = 0; u2 < peak1; u2++) {
                for (int v2 = 0; v2 < peak2; v2++) {
                    int j = u2 * peak2 + v2;
                    if (matrix[i][j] == 1 && i != j) {
                        printf("(%d,%d) ", u2 + 1, v2 + 1);
                        hasNeighbors = 1;
                    }
                }
            }
            if (!hasNeighbors) {
                printf("нет соседей");
            }
            printf("\n");
        }
    }
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));
    int peak1, peak2;
    printf("Введите количество вершин для графа G1: ");
    scanf("%d", &peak1);
    printf("Введите количество вершин для графа G2: ");
    scanf("%d", &peak2);

    if (peak1 <= 0 || peak2 <= 0) {
        printf("Ошибка: количество вершин должно быть положительным числом.\n");
        return 1;
    }

    int** matrix1 = createMatrixFunc(peak1);
    int** matrix2 = createMatrixFunc(peak2);

    createMatrix(matrix1, peak1);
    createMatrix(matrix2, peak2);

    printf("\nИсходные графы\n");
    printMatrix(matrix1, peak1, "Матрица смежности G1");
    printMatrix(matrix2, peak2, "Матрица смежности G2");

    int resultPeak;
    int** result = cartesianProduct(matrix1, peak1, matrix2, peak2, &resultPeak);

    printf("\nДекартово произведение G1 X G2\n");
    printf("Количество вершин в результате: %d x %d = %d\n\n", peak1, peak2, resultPeak);

    printCartesianMatrix(result, peak1, peak2, "Матрица смежности декартова произведения");

    printCartesianAdjacencyList(result, peak1, peak2);

    freem(matrix1, peak1);
    freem(matrix2, peak2);
    freem(result, resultPeak);
    return 0;
}
