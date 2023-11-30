#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *nazov;
    int rok;
    int serie;
    int epizody;
} TSerialy;

int pocetSerialov;

int porovnajPodlaRoku(const void *a, const void *b) {
    return ((TSerialy *)a)->rok - ((TSerialy *)b)->rok;
}

int porovnajPodlaPocetuDielov(const void *a, const void *b) {
    return ((TSerialy *)a)->epizody - ((TSerialy *)b)->epizody;
}

void zapisSerialyDoSuboru(TSerialy *serialy, const char *zoznam, char pismeno) {
    FILE *subor = fopen(zoznam, "w");
    if (subor == NULL) {
        printf("Chyba pri otvarani suboru pre zapis\n");
        return;
    }

    fprintf(subor, "Seriály s názvom zaèínajúcim na '%c' a vydane pred rokom 2000:\n", pismeno);

    for (int i = 0; i < pocetSerialov; i++) {
        if (serialy[i].nazov[0] == pismeno && serialy[i].rok < 2000) {
            fprintf(subor, "%s %d %d %d\n", serialy[i].nazov, serialy[i].rok, serialy[i].serie, serialy[i].epizody);
        }
    }

    fclose(subor);
}

int main() {
    FILE *subor = fopen("serialy.txt", "r");
    if (subor == NULL) {
        printf("Chyba pri otvarani suboru\n");
        return 1;
    }

    if (fscanf(subor, "%d", &pocetSerialov) != 1) {
        printf("Chyba pri citani poctu serialov zo suboru\n");
        fclose(subor);
        return 1;
    }

    TSerialy *serialy = (TSerialy *)malloc(pocetSerialov * sizeof(TSerialy));
    if (serialy == NULL) {
        printf("Chyba pri alokacii pamate\n");
        fclose(subor);
        return 1;
    }

    for (int i = 0; i < pocetSerialov; i++) {
        serialy[i].nazov = malloc(50 * sizeof(char)); // Ak má by názov maximálne 50 znakov dlhý
        if (serialy[i].nazov == NULL) {
            printf("Chyba pri alokacii pamate pre nazov seriálu\n");
            fclose(subor);
            free(serialy);
            return 1;
        }

        if (fscanf(subor, "%s %d %d %d", serialy[i].nazov, &serialy[i].rok, &serialy[i].serie, &serialy[i].epizody) != 4) {
            printf("Chyba pri citani udajov o seriáloch zo suboru\n");
            fclose(subor);
            free(serialy[i].nazov);
            free(serialy);
            return 1;
        }
    }

    fclose(subor);

    printf("Nacitane udaje o serialoch:\n");
    for (int i = 0; i < pocetSerialov; i++) {
        printf("%s %d %d %d\n", serialy[i].nazov, serialy[i].rok, serialy[i].serie, serialy[i].epizody);
    }

    int kriterium;
    do {
        printf("Zoradit podla:\n1 - Rok vydania\n2 - Pocet dielov\nVyberte moznost: ");
        if (scanf("%d", &kriterium) != 1 || (kriterium != 1 && kriterium != 2)) {
            printf("Zadali ste nespravny vstup. Skuste znovu.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);

    if (kriterium == 1) {
        qsort(serialy, pocetSerialov, sizeof(TSerialy), porovnajPodlaRoku);
    } else if (kriterium == 2) {
        qsort(serialy, pocetSerialov, sizeof(TSerialy), porovnajPodlaPocetuDielov);
    }

    printf("Zoradene udaje o serialoch:\n");
    for (int i = 0; i < pocetSerialov; i++) {
        printf("%s %d %d %d\n", serialy[i].nazov, serialy[i].rok, serialy[i].serie, serialy[i].epizody);
    }

    int hladanyRok;
    printf("Zadajte rok pre vyhladavanie: ");
    if (scanf("%d", &hladanyRok) != 1) {
        printf("Nespravny vstup.\n");
        free(serialy);
        return 1;
    }
    qsort(serialy, pocetSerialov, sizeof(TSerialy), porovnajPodlaRoku);

    TSerialy hladanySerial = {"", hladanyRok, 0, 0};
    TSerialy *match = bsearch(&hladanySerial, serialy, pocetSerialov, sizeof(TSerialy), porovnajPodlaRoku);

    if (match != NULL) {
        TSerialy *aktualny = match;
        while (aktualny >= serialy && aktualny->rok == hladanyRok) {
            printf("Najdeny serial: %s %d %d %d\n", aktualny->nazov, aktualny->rok, aktualny->serie, aktualny->epizody);
            aktualny--;
        }

        aktualny = match + 1;
        while (aktualny < serialy + pocetSerialov && aktualny->rok == hladanyRok) {
            printf("Najdeny serial: %s %d %d %d\n", aktualny->nazov, aktualny->rok, aktualny->serie, aktualny->epizody);
            aktualny++;
        }
    } else {
        printf("Ziadne serialy s rokom %d neboli najdene.\n", hladanyRok);
    }

    char hladanePismeno;
    printf("Zadajte pismeno pre vyhladavanie: ");
    if (scanf(" %c", &hladanePismeno) != 1) {
        printf("Nespravny vstup.\n");
        free(serialy);
        return 1;
    }

    zapisSerialyDoSuboru(serialy, "vyfiltrovane_serialy.txt", hladanePismeno);
    free(serialy);

    return 0;
}
