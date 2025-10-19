#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
int main() {
    setlocale(LC_ALL, "Russian");
    int VERSHINY;
    printf("введите количество вершин: ");
    scanf_s("%d", &VERSHINY); // пользователь вводит с клавы + динамические + характеристики по матрице инцендентности 
    int MAXREBER = VERSHINY * VERSHINY;
    int** matr_smezh = malloc(VERSHINY * sizeof(int*));
    for (int i = 0; i < VERSHINY; i++)
        matr_smezh[i] = calloc(VERSHINY, sizeof(int));
    int* stepen_smezh = calloc(VERSHINY, sizeof(int));
    int* petlya = calloc(VERSHINY, sizeof(int));
    int* svyaz_u = malloc(MAXREBER * sizeof(int));
    int* svyaz_v = malloc(MAXREBER * sizeof(int));
    int kol_reber = 0;
    srand(time(NULL));
    for (int i = 0; i < VERSHINY; i++) {
        for (int j = i; j < VERSHINY; j++) {
            if (i == j && rand() % 5 < 2) matr_smezh[i][i] = 1;
            else if (i != j && rand() % 5 < 2) {
                matr_smezh[i][j] = matr_smezh[j][i] = 1;
            }
        }
    }
    for (int i = 0; i < VERSHINY; i++) {
        for (int j = i; j < VERSHINY; j++) {
            if (matr_smezh[i][j]) {
                svyaz_u[kol_reber] = i;
                svyaz_v[kol_reber] = j;
                kol_reber++;
            }
        }
    }
    int maks_stepen_smezh = 0;
    for (int i = 0; i < VERSHINY; i++) {
        petlya[i] = matr_smezh[i][i];
        int summa = 0;
        for (int j = 0; j < VERSHINY; j++)
            summa += matr_smezh[i][j];
        stepen_smezh[i] = summa + petlya[i];
        if (stepen_smezh[i] > maks_stepen_smezh)
            maks_stepen_smezh = stepen_smezh[i];
    }
    int** matr_inc = malloc(VERSHINY * sizeof(int*));
    for (int i = 0; i < VERSHINY; i++)
        matr_inc[i] = calloc(kol_reber, sizeof(int));
    for (int v = 0; v < VERSHINY; v++) {
        for (int r = 0; r < kol_reber; r++) {
            if (svyaz_u[r] == svyaz_v[r])
                matr_inc[v][r] = (v == svyaz_u[r]) ? 2 : 0;
            else
                matr_inc[v][r] = (v == svyaz_u[r] || v == svyaz_v[r]) ? 1 : 0;
        }
    }
    printf("\nколичество вершин: %d\n", VERSHINY);
    printf("размер графа (число рёбер): %d\n\n", kol_reber);
    printf("матрица смежности:\n");
    for (int i = 0; i < VERSHINY; i++) {
        for (int j = 0; j < VERSHINY; j++)
            printf("%2d ", matr_smezh[i][j]);
        printf("\n");
    }
    printf("\nматрица инцидентности:\n");
    for (int i = 0; i < VERSHINY; i++) {
        for (int j = 0; j < kol_reber; j++)
            printf("%2d ", matr_inc[i][j]);
        printf("\n");
    }
    printf("\n i |");
    for (int j = 0; j < VERSHINY; j++) printf(" %d", j);
    printf(" | петля | степень\n");
    printf("    ");
    for (int j = 0; j < VERSHINY; j++) printf("  ");
    printf("\n");
    for (int i = 0; i < VERSHINY; i++) {
        printf(" %d |", i);
        for (int j = 0; j < VERSHINY; j++)
            printf(" %d", matr_smezh[i][j]);
        printf(" |   %d   |   %d\n", petlya[i], stepen_smezh[i]);
    }
    printf("\nхарактеристики по матрице смежности:\n");
    for (int i = 0; i < VERSHINY; i++) {
        printf("вершина %d: степень = %d, петли = %d\n", i, stepen_smezh[i], petlya[i]);
    }
    printf("\nтипы вершин по матрице смежности:\n");
    for (int i = 0; i < VERSHINY; i++) {
        if (stepen_smezh[i] == 0)
            printf("вершина %d — изолированная\n", i);
        else if (stepen_smezh[i] == 1)
            printf("вершина %d — концевая\n", i);
        else if (stepen_smezh[i] == maks_stepen_smezh)
            printf("вершина %d — доминирующая\n", i);
        else if (petlya[i])
            printf("вершина %d — с петлёй\n", i);
        else
            printf("вершина %d — обычная\n", i);
    }
    int* stepen_inc = calloc(VERSHINY, sizeof(int));
    int* petlya_inc = calloc(VERSHINY, sizeof(int));
    int max_stepen_inc = 0;

    // инциндентность

    for (int i = 0; i < VERSHINY; i++) {
        int deg = 0, loops = 0;
        for (int j = 0; j < kol_reber; j++) {
            if (matr_inc[i][j] == 1)
                deg++;
            else if (matr_inc[i][j] == 2) {
                deg += 2;
                loops++;
            }
        }
        stepen_inc[i] = deg;
        petlya_inc[i] = loops;
        if (deg > max_stepen_inc)
            max_stepen_inc = deg;
    }
    printf("\nхарактеристики по матрице инцидентности:\n");
    for (int i = 0; i < VERSHINY; i++) {
        printf("вершина %d: степень = %d, петли = %d\n", i, stepen_inc[i], petlya_inc[i]);
    }
    printf("\nтипы вершин по матрице инцидентности:\n");
    for (int i = 0; i < VERSHINY; i++) {
        if (stepen_inc[i] == 0)
            printf("вершина %d — изолированная\n", i);
        else if (stepen_inc[i] == 1)
            printf("вершина %d — концевая\n", i);
        else if (stepen_inc[i] == max_stepen_inc)
            printf("вершина %d — доминирующая\n", i);
        else if (petlya_inc[i])
            printf("вершина %d — с петлёй\n", i);
        else
            printf("Вершина %d — обычная\n", i);
    }
    for (int i = 0; i < VERSHINY; i++) {
        free(matr_smezh[i]);
        free(matr_inc[i]);
    }
    free(matr_smezh);
    free(matr_inc);
    free(stepen_smezh);
    free(petlya);
    free(svyaz_u);
    free(svyaz_v);
    free(stepen_inc);
    free(petlya_inc);
    return 0;
}

