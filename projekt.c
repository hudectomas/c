#include <stdio.h>
#include <stdlib.h>

// Uloha 1
typedef struct {
    char nazov[50];
    int rok;
    int pocet_serii;
    int pocet_dielov;
} TSerial;

TSerial* nacitaj_serialy(const char* meno_suboru, int* pocet) {
    FILE* subor = fopen(meno_suboru, "r");
    if (subor == NULL) {
        printf("Nepodarilo sa otvorit subor.\n");
        return NULL;
    }

    fscanf(subor, "%d", pocet);
    TSerial* serialy = malloc(*pocet * sizeof(TSerial));

    for (int i = 0; i < *pocet; i++) {
        fscanf(subor, "%s %d %d %d", serialy[i].nazov, &serialy[i].rok, &serialy[i].pocet_serii, &serialy[i].pocet_dielov);
    }

    fclose(subor);
    return serialy;
}

void vypis_serialy(TSerial* serialy, int pocet) {
    for (int i = 0; i < pocet; i++) {
        printf("%s %d %d %d\n", serialy[i].nazov, serialy[i].rok, serialy[i].pocet_serii, serialy[i].pocet_dielov);
    }
}

// Uloha 2
int porovnaj_podla_roka(const void* a, const void* b) {
    TSerial* serial_a = (TSerial*)a;
    TSerial* serial_b = (TSerial*)b;
    return serial_a->rok - serial_b->rok;
}

int porovnaj_podla_poctu_dielov(const void* a, const void* b) {
    TSerial* serial_a = (TSerial*)a;
    TSerial* serial_b = (TSerial*)b;
    return (serial_a->pocet_serii * serial_a->pocet_dielov) - (serial_b->pocet_serii * serial_b->pocet_dielov);
}

void usporiadaj_serialy(TSerial* serialy, int pocet, int (*porovnaj)(const void*, const void*)) {
    qsort(serialy, pocet, sizeof(TSerial), porovnaj);
}

// Uloha 3
void vypis_serialy_z_roka(TSerial* serialy, int pocet, int rok) {
    TSerial kluc = {.rok = rok};
    TSerial* serial = (TSerial*)bsearch(&kluc, serialy, pocet, sizeof(TSerial), porovnaj_podla_roka);

    if (serial == NULL || serial->rok != rok) {
        printf("Chyba: Zadany rok %d sa nenachadza v subore.\n", rok);
        return;
    }
    else{
        printf("Serialy z roku %d:\n",rok);
    }

    int hladaj = 0;

    while (serial != NULL && serial->rok == rok) {

        printf("%s\n", serial->nazov);
        serial++;
        hladaj = 1;
    }

}

// Uloha 4
void zapis_serialy_do_suboru(TSerial* serialy, int pocet, const char* meno_suboru, char pismeno) {
    FILE* subor = fopen(meno_suboru, "w");
    if (subor == NULL) {
        printf("Nepodarilo sa otvorit subor.\n");
        return;
    }

    for (int i = 0; i < pocet; i++) {
        if (serialy[i].nazov[0] == pismeno && serialy[i].rok < 2000) {
            fprintf(subor, "%s %d %d %d\n", serialy[i].nazov, serialy[i].rok, serialy[i].pocet_serii, serialy[i].pocet_dielov);
        }
    }

    fclose(subor);
}

int main() {
    int pocet;
    TSerial* serialy = nacitaj_serialy("serialy.txt", &pocet);

    printf("Nacitane serialy:\n");
    vypis_serialy(serialy, pocet);

    char option;
    printf("\nVyber moznost (a - usporiadane podla roka, b - usporiadane podla poctu dielov): ");
    scanf(" %c", &option);

    switch (option) {
        case 'a':
            usporiadaj_serialy(serialy, pocet, porovnaj_podla_roka);
            printf("\nSerialy usporiadane podla roka:\n");
            vypis_serialy(serialy, pocet);
            break;
        case 'b':
            usporiadaj_serialy(serialy, pocet, porovnaj_podla_poctu_dielov);
            printf("\nSerialy usporiadane podla poctu dielov:\n");
            vypis_serialy(serialy, pocet);
            break;
        default:
            printf("\nNeplatna moznost.\n");
            break;
    }

    int hladany_rok;
    printf("\nZadaj rok pre hladanie serialov: ");
    scanf("%d", &hladany_rok);

    vypis_serialy_z_roka(serialy, pocet, hladany_rok);

    zapis_serialy_do_suboru(serialy, pocet, "serialy_pred_2000.txt", 'S');

    free(serialy);
    return 0;
}

