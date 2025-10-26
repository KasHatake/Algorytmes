#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

typedef struct Uzol {
    int vershina;
    struct Uzol* sleduyuschiy;
} Uzol;

typedef struct {
    int* dannye;
    int verx;
    int vmestimost;
} Stek;

int* poseshennye;
Uzol** spisok_smezhnosti;
int** matrica_smezhnosti;
int kolichestvo_vershin;

Stek* sozdat_stek(int vmestimost) {
    Stek* stek = (Stek*)malloc(sizeof(Stek));
    stek->dannye = (int*)malloc(vmestimost * sizeof(int));
    stek->verx = -1;
    stek->vmestimost = vmestimost;
    return stek;
}
void osvobodit_stek(Stek* stek) {
    free(stek->dannye);
    free(stek);
}
void podnyat_v_stek(Stek* stek, int versh) {
    if (stek->verx < stek->vmestimost - 1)
        stek->dannye[++stek->verx] = versh;
}
int izvlech_iz_steka(Stek* stek) {
    if (stek->verx >= 0)
        return stek->dannye[stek->verx--];
    return -1;
}
int pustoy_stek(Stek* stek) {
    return stek->verx == -1;
}

void intisializaciya_grafa(int n) {
    kolichestvo_vershin = n;
    matrica_smezhnosti = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrica_smezhnosti[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) matrica_smezhnosti[i][j] = 0;
    }
    spisok_smezhnosti = (Uzol**)malloc(n * sizeof(Uzol*));
    for (int i = 0; i < n; i++) spisok_smezhnosti[i] = NULL;
    poseshennye = (int*)malloc(n * sizeof(int));
}
void ochistit_graf() {
    if (matrica_smezhnosti) {
        for (int i = 0; i < kolichestvo_vershin; i++) free(matrica_smezhnosti[i]);
        free(matrica_smezhnosti); matrica_smezhnosti = NULL;
    }
    if (spisok_smezhnosti) {
        for (int i = 0; i < kolichestvo_vershin; i++) {
            Uzol* curr = spisok_smezhnosti[i];
            while (curr) { Uzol* tmp = curr; curr = curr->sleduyuschiy; free(tmp); }
        }
        free(spisok_smezhnosti); spisok_smezhnosti = NULL;
    }
    if (poseshennye) { free(poseshennye); poseshennye = NULL; }
}
void dobavit_rebro(int ot, int k) {
    Uzol* nov = (Uzol*)malloc(sizeof(Uzol));
    nov->vershina = k; nov->sleduyuschiy = spisok_smezhnosti[ot];
    spisok_smezhnosti[ot] = nov;
    nov = (Uzol*)malloc(sizeof(Uzol));
    nov->vershina = ot; nov->sleduyuschiy = spisok_smezhnosti[k];
    spisok_smezhnosti[k] = nov;
}
void generacija_sluchaynogo_grafa() {
    for (int i = 0; i < kolichestvo_vershin; i++)
        for (int j = 0; j < kolichestvo_vershin; j++) matrica_smezhnosti[i][j] = 0;
    for (int i = 0; i < kolichestvo_vershin; i++) {
        Uzol* curr = spisok_smezhnosti[i];
        while (curr) { Uzol* tmp = curr; curr = curr->sleduyuschiy; free(tmp); }
        spisok_smezhnosti[i] = NULL;
    }
    for (int i = 0; i < kolichestvo_vershin; i++)
        for (int j = i + 1; j < kolichestvo_vershin; j++)
            if (rand() % 2 == 1) {
                matrica_smezhnosti[i][j] = 1;
                matrica_smezhnosti[j][i] = 1;
                dobavit_rebro(i, j);
            }
}

void glubina_rekursiya_matrica(int start_versh) {
    poseshennye[start_versh] = 1;
    printf("%d ", start_versh + 1);
    for (int i = 0; i < kolichestvo_vershin; i++)
        if (matrica_smezhnosti[start_versh][i] && !poseshennye[i])
            glubina_rekursiya_matrica(i);
}
void glubina_bez_rekursii_matrica(int start_versh) {
    Stek* stek = sozdat_stek(kolichestvo_vershin);
    int* pos_local = (int*)malloc(kolichestvo_vershin * sizeof(int));
    for (int i = 0; i < kolichestvo_vershin; i++) pos_local[i] = 0;
    podnyat_v_stek(stek, start_versh); pos_local[start_versh] = 1;
    printf("Obhod glubina (matrica, bez rekursii): ");
    while (!pustoy_stek(stek)) {
        int curr = izvlech_iz_steka(stek); printf("%d ", curr + 1);
        for (int i = 0; i < kolichestvo_vershin; i++)
            if (matrica_smezhnosti[curr][i] && !pos_local[i]) {
                podnyat_v_stek(stek, i); pos_local[i] = 1;
            }
    }
    printf("\n");
    free(pos_local); osvobodit_stek(stek);
}
void glubina_rekursiya_spisok(int start_versh) {
    poseshennye[start_versh] = 1;
    printf("%d ", start_versh + 1);
    Uzol* tmp = spisok_smezhnosti[start_versh];
    while (tmp) {
        int smezhnaya = tmp->vershina;
        if (!poseshennye[smezhnaya])
            glubina_rekursiya_spisok(smezhnaya);
        tmp = tmp->sleduyuschiy;
    }
}
void glubina_bez_rekursii_spisok(int start_versh) {
    Stek* stek = sozdat_stek(kolichestvo_vershin);
    int* pos_local = (int*)malloc(kolichestvo_vershin * sizeof(int));
    for (int i = 0; i < kolichestvo_vershin; i++) pos_local[i] = 0;
    podnyat_v_stek(stek, start_versh); pos_local[start_versh] = 1;
    printf("Obhod glubina (spisok, bez rekursii): ");
    while (!pustoy_stek(stek)) {
        int curr = izvlech_iz_steka(stek); printf("%d ", curr + 1);
        Stek* temp_stek = sozdat_stek(kolichestvo_vershin);
        Uzol* tmp = spisok_smezhnosti[curr];
        while (tmp) {
            int smezhnaya = tmp->vershina;
            if (!pos_local[smezhnaya]) {
                podnyat_v_stek(temp_stek, smezhnaya);
                pos_local[smezhnaya] = 1;
            }
            tmp = tmp->sleduyuschiy;
        }
        while (!pustoy_stek(temp_stek))
            podnyat_v_stek(stek, izvlech_iz_steka(temp_stek));
        osvobodit_stek(temp_stek);
    }
    printf("\n");
    free(pos_local); osvobodit_stek(stek);
}
void print_matrica() {
    printf("\nMatrica smezhnosti:\n   ");
    for (int i = 0; i < kolichestvo_vershin; i++)
        printf("%2d ", i + 1);
    printf("\n");
    for (int i = 0; i < kolichestvo_vershin; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < kolichestvo_vershin; j++)
            printf("%2d ", matrica_smezhnosti[i][j]);
        printf("\n");
    }
}
void print_spisok() {
    printf("\nSpisok smezhnosti:\n");
    for (int i = 0; i < kolichestvo_vershin; i++) {
        printf("%d: ", i + 1);
        Uzol* tmp = spisok_smezhnosti[i];
        while (tmp) {
            printf("%d ", tmp->vershina + 1);
            tmp = tmp->sleduyuschiy;
        }
        printf("\n");
    }
}
void initsializaciya_poseshennyh() {
    for (int i = 0; i < kolichestvo_vershin; i++) poseshennye[i] = 0;
}
void menyu() {
    printf("1 - Peresozdat graf\n");
    printf("2 - Rekursivny obhod (matrica)\n");
    printf("3 - Bez rekursii (matrica)\n");
    printf("4 - Rekursivny obhod (spisok)\n");
    printf("5 - Bez rekursii (spisok)\n");
    printf("0 - Vihod\n");
    printf("Vash vibor: ");
}

int main() {
    setlocale(LC_ALL, "ru"); srand(time(NULL));
    int vibor, start, graf_sozdan = 0;
    while (1) {
        if (!graf_sozdan) {
            printf("Vvedite kol-vo vershin graf:\n");
            scanf("%d", &kolichestvo_vershin);
            if (kolichestvo_vershin <= 0) { printf("Oshibka!\n"); continue; }
            intisializaciya_grafa(kolichestvo_vershin);
            generacija_sluchaynogo_grafa();
            graf_sozdan = 1;
            printf("Graf sozdany!\n");
            print_matrica(); print_spisok();
        }
        menyu(); scanf("%d", &vibor);
        switch (vibor) {
        case 0: ochistit_graf(); return 0;
        case 1: ochistit_graf(); graf_sozdan = 0; break;
        case 2:
            printf("Vvedite start vershinu: "); scanf("%d", &start);
            if (start < 1 || start > kolichestvo_vershin) { printf("Nom ne v diapazone!\n"); break; }
            initsializaciya_poseshennyh(); printf("Obhod glubina (matrica, rekurs): ");
            glubina_rekursiya_matrica(start - 1); printf("\n"); break;
        case 3:
            printf("Vvedite start vershinu: "); scanf("%d", &start);
            if (start < 1 || start > kolichestvo_vershin) { printf("Nom ne v diapazone!\n"); break; }
            glubina_bez_rekursii_matrica(start - 1); break;
        case 4:
            printf("Vvedite start vershinu: "); scanf("%d", &start);
            if (start < 1 || start > kolichestvo_vershin) { printf("Nom ne v diapazone!\n"); break; }
            initsializaciya_poseshennyh(); printf("Obhod glubina (spisok, rekurs): ");
            glubina_rekursiya_spisok(start - 1); printf("\n"); break;
        case 5:
            printf("Vvedite start vershinu: "); scanf("%d", &start);
            if (start < 1 || start > kolichestvo_vershin) { printf("Nom ne v diapazone!\n"); break; }
            glubina_bez_rekursii_spisok(start - 1); break;
        default:
            printf("Oshibka!\n"); break;
        }
    }
    return 0;
}
