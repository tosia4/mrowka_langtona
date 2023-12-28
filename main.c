#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BIALY 1
#define CZARNY 0

#define GORA 0
#define DOL 1
#define PRAWO 2
#define LEWO 3


/*
 * zaczelam cos pisac, na razie wszystko w jednym pliku, najwyzej potem podzielimy
 *
 *
 */

struct plansza{
    int wiersze;
    int kolumny;
    int **pola;
};

struct mrowka{
    int x;
    int y;
    int kierunek;
};

struct mrowka* tworzenie_morowki(int x, int y, int kierunek){
    struct mrowka* nowa_mrowka = malloc(sizeof(struct mrowka));

    if (nowa_mrowka == NULL){
        return NULL;
    }

    nowa_mrowka->x = x;
    nowa_mrowka->y = y;
    nowa_mrowka->kierunek = kierunek;

    return nowa_mrowka;

}




struct plansza* tworzenie_planszy(int wiersze, int kolumny){
    struct plansza* nowa_plansza = malloc(sizeof(struct plansza));

    if (nowa_plansza == NULL){
        return NULL;
    }

    nowa_plansza->wiersze = wiersze;
    nowa_plansza->kolumny = kolumny;

    int **pola = malloc(sizeof(int*) * wiersze);

    if (pola == NULL){
        free (nowa_plansza);
        return NULL;
    }

    for (int i =0; i < wiersze; i++){
        *(pola + i) = calloc(sizeof(int) , kolumny);

        if(*(pola + i) == NULL){

            for (int j = 0; j < i; ++j) { //zwalnianie wierszy
                free(*(pola + j));
            }

            free(pola);
            free(nowa_plansza);

            return NULL;

        }

    }

    nowa_plansza->pola = pola;

    return nowa_plansza;

}

void wyswietl(struct plansza* plansza){

    for (int i = 0; i < plansza->wiersze; ++i) {
        for (int j = 0; j < plansza->kolumny; ++j) {
            printf("%d ", plansza->pola[i][j]);
        }
        printf("\n");
    }


}

void zwolnij_plansze(struct plansza* plansza){
    for (int i = 0; i < plansza->wiersze; ++i) {
        free(plansza->pola[i]);
    }
    free(plansza->pola);
    free(plansza);
}


int main() {

    struct plansza* plansza = tworzenie_planszy(10,20);

    wyswietl(plansza);

    zwolnij_plansze(plansza);
}
