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

void printMatrix(int** matrix, int peak) {
    printf("Матрица смежности:\n");
    for (int i = 0; i < peak; i++) {
        for (int j = 0; j < peak; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void makeList(int** matrix, int peak) {
    printf("Список смежности:\n");
    for (int i = 0; i < peak; i++) {
        printf("Вершина %d: ", i + 1);
        int near = 0;
        for (int j = 0; j < peak; j++) {
            if (matrix[i][j] == 1 && i != j) {
                printf("%d ", j + 1);
                near = 1;
            }
        }
        if (!near) {
            printf("Нет соседей");
        }
        printf("\n");
    }
    printf("\n");
}

void freem(int** matrix, int peak) {
    for (int i = 0; i < peak; i++) {
        free(matrix[i]);
    }
    free(matrix);
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
        printf("Ошибка: количество вершин должно быть натуральным числом.\n");
        return 1;
    }

    int** M1 = (int**)malloc(peak1 * sizeof(int*));
    for (int i = 0; i < peak1; i++) {
        M1[i] = (int*)malloc(peak1 * sizeof(int));
    }

    int** M2 = (int**)malloc(peak2 * sizeof(int*));
    for (int i = 0; i < peak2; i++) {
        M2[i] = (int*)malloc(peak2 * sizeof(int));
    }

    printf("\nГраф G1\n");
    createMatrix(M1, peak1);
    printMatrix(M1, peak1);
    makeList(M1, peak1);

    printf("Граф G2\n");
    createMatrix(M2, peak2);
    printMatrix(M2, peak2);
    makeList(M2, peak2);

    freem(M1, peak1);
    freem(M2, peak2);
    return 0;
}
