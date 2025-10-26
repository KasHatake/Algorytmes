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

int** copyMatrix(int** source, int peak) {
    int** copy = createMatrixFunc(peak);
    for (int i = 0; i < peak; i++) {
        for (int j = 0; j < peak; j++) {
            copy[i][j] = source[i][j];
        }
    }
    return copy;
}

int** unionGraphs(int** matrix1, int** matrix2, int peak) {
    int** result = createMatrixFunc(peak);
    for (int i = 0; i < peak; i++) {
        for (int j = 0; j < peak; j++) {
            result[i][j] = (matrix1[i][j] || matrix2[i][j]) ? 1 : 0;
        }
    }
    return result;
}

int** intersectionGraphs(int** matrix1, int** matrix2, int peak) {
    int** result = createMatrixFunc(peak);
    for (int i = 0; i < peak; i++) {
        for (int j = 0; j < peak; j++) {
            result[i][j] = (matrix1[i][j] && matrix2[i][j]) ? 1 : 0;
        }
    }
    return result;
}

int** ringSumGraphs(int** matrix1, int** matrix2, int peak) {
    int** result = createMatrixFunc(peak);
    for (int i = 0; i < peak; i++) {
        for (int j = 0; j < peak; j++) {
            result[i][j] = (matrix1[i][j] != matrix2[i][j]) ? 1 : 0;
        }
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "ru");
    srand(time(NULL));
    int peak;
    printf("Введите количество вершин для графов G1 и G2: ");
    scanf("%d", &peak);

    if (peak <= 0) {
        printf("Ошибка: количество вершин должно быть положительным числом.\n");
        return 1;
    }

    int** matrix1 = createMatrixFunc(peak);
    int** matrix2 = createMatrixFunc(peak);

    createMatrix(matrix1, peak);
    createMatrix(matrix2, peak);

    printf("\nИсходные графы\n");
    printMatrix(matrix1, peak, "Матрица смежности G1");
    printMatrix(matrix2, peak, "Матрица смежности G2");

    int choice;
    do {
        printf("\nОперации над графами\n");
        printf("1 - Объединение G1 U G2\n");
        printf("2 - Пересечение G1 n G2\n");
        printf("3 - Сложение по модулю (2) G1 (+) G2\n");
        printf("0 - Выход\n");
        printf("Выберите операцию: ");
        scanf("%d", &choice);

        int** result = NULL;
        const char* operationName = "";

        switch (choice) {
        case 1:
            result = unionGraphs(matrix1, matrix2, peak);
            operationName = "Результат объединения G1 U G2";
            break;
        case 2:
            result = intersectionGraphs(matrix1, matrix2, peak);
            operationName = "Результат пересечения G1 n G2";
            break;
        case 3:
            result = ringSumGraphs(matrix1, matrix2, peak);
            operationName = "Результат сложения по модулю (2) G1 (+) G2";
            break;
        case 0:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор!\n");
        }

        if (result != NULL) {
            printf("\n%s\n", operationName);
            printMatrix(result, peak, "");
            freem(result, peak);
        }
    } while (choice != 0);

    freem(matrix1, peak);
    freem(matrix2, peak);
    return 0;
}
