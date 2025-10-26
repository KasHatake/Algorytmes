#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAXV 50
int matrica[MAXV][MAXV];
int kolichestvo_vershin;

void print_matrica() {
    printf("\nMatrica smezhnosti:\n   ");
    for (int i = 0; i < kolichestvo_vershin; i++)
        printf("%2d ", i + 1);
    printf("\n");
    for (int i = 0; i < kolichestvo_vershin; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < kolichestvo_vershin; j++)
            printf("%2d ", matrica[i][j]);
        printf("\n");
    }
}
void otogdestvlenie_vershin_matrica(int v1, int v2) {
    for (int i = 0; i < kolichestvo_vershin; i++) {
        if (i != v1 && i != v2) {
            matrica[v1][i] |= matrica[v2][i];
            matrica[i][v1] |= matrica[i][v2];
        }
    }
    for (int i = v2; i < kolichestvo_vershin - 1; i++)
        for (int j = 0; j < kolichestvo_vershin; j++) matrica[i][j] = matrica[i + 1][j];
    for (int j = v2; j < kolichestvo_vershin - 1; j++)
        for (int i = 0; i < kolichestvo_vershin; i++) matrica[i][j] = matrica[i][j + 1];
    kolichestvo_vershin--;
}

void stjagivanie_rebra_matrica(int v1, int v2) {
    matrica[v1][v2] = 0;
    matrica[v2][v1] = 0;
}
void rasscheplenie_vershiny_matrica(int v) {
    int t = kolichestvo_vershin;
    for (int i = 0; i < kolichestvo_vershin; i++) {
        matrica[t][i] = matrica[v][i];
        matrica[i][t] = matrica[i][v];
    }
    kolichestvo_vershin++;
}
void glubina_rekursiya_matrica(int v, int* posesh) {
    posesh[v] = 1;
    printf("%d ", v + 1);
    for (int i = 0; i < kolichestvo_vershin; i++)
        if (matrica[v][i] && !posesh[i])
            glubina_rekursiya_matrica(i, posesh);
}
void glubina_bez_rekursii_matrica(int start) {
    int* pos_local = (int*)malloc(kolichestvo_vershin * sizeof(int));
    for (int i = 0; i < kolichestvo_vershin; i++) pos_local[i] = 0;
    int* stek = (int*)malloc(kolichestvo_vershin * sizeof(int));
    int verx = -1;

    stek[++verx] = start; pos_local[start] = 1;
    printf("Obhod glubina (matrica, bez rekursii): ");

    while (verx >= 0) {
        int curr = stek[verx--];
        printf("%d ", curr + 1);
        for (int i = kolichestvo_vershin - 1; i >= 0; i--)
            if (matrica[curr][i] && !pos_local[i]) {
                stek[++verx] = i;
                pos_local[i] = 1;
            }
    }
    printf("\n");
    free(stek);
    free(pos_local);
}

int main() {
    printf("Vvedite kol-vo vershin (do %d): ", MAXV);
    scanf("%d", &kolichestvo_vershin);
    printf("Vvodite matrixu smeshnosti:\n");
    for (int i = 0; i < kolichestvo_vershin; i++)
        for (int j = 0; j < kolichestvo_vershin; j++)
            scanf("%d", &matrica[i][j]);
    int vibor, a, b;
    while (1) {
        printf("\nMENU:\n1- Otogdestvlenie\n2- Stjagivanie\n3- Rasscheplenie\n4- Rekursivno toest' preobtazovanie \n5- Bez rekursii\n0- Vihod\nVibor: ");
        scanf("%d", &vibor);
        switch (vibor) {
        case 1:
            printf("Vvedite dve vershiny: "); scanf("%d%d", &a, &b);
            otogdestvlenie_vershin_matrica(a - 1, b - 1); print_matrica(); break;
        case 2:
            printf("Vvedite dve vershiny: "); scanf("%d%d", &a, &b);
            stjagivanie_rebra_matrica(a - 1, b - 1); print_matrica(); break;
        case 3:
            printf("Vvedite vershinu: "); scanf("%d", &a);
            rasscheplenie_vershiny_matrica(a - 1); print_matrica(); break;
        case 4:
            printf("Vvedite startovuyu vershinu: "); scanf("%d", &a);
            {
                int* posesh = (int*)malloc(kolichestvo_vershin * sizeof(int));
                for (int i = 0; i < kolichestvo_vershin; i++) posesh[i] = 0;
                printf("Obhod glubina (matrica, rekursivno): ");
                glubina_rekursiya_matrica(a - 1, posesh);
                printf("\n"); free(posesh);
            }
            break;
        case 5:
            printf("Vvedite startovuyu vershinu: "); scanf("%d", &a);
            glubina_bez_rekursii_matrica(a - 1);
            break;
        case 0: return 0;
        default: printf("Oshibka!\n");
        }
    }
    return 0;
}
