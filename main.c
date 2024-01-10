#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

#define BIALY 0x25a1
#define CZARNY 0x25a0

#define GORA 0x25b3
#define DOL 0x25bd
#define PRAWO 0x25b7
#define LEWO 0x25c1

#define RAMKA_PIONOWA 0x2503
#define RAMKA_POZIOMA 0x2501
#define ROG_GORA_LEWO 0x250f
#define ROG_GORA_PRAWO 0x2513
#define ROG_DOL_LEWO 0x2517
#define ROG_DOL_PRAWO 0x251b

/*
 * zaczelam cos pisac, na razie wszystko w jednym pliku, najwyzej potem podzielimy
 *
 *
 */

struct plansza{
    int wiersze;
    int kolumny;
    wchar_t **pola;
};

struct mrowka{
    int x;
    int y;
    wchar_t kierunek;
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

    int **pola = malloc(sizeof(wchar_t*) * wiersze);

    if (pola == NULL){
        free (nowa_plansza);
        return NULL;
    }

    for (int i =0; i < wiersze; i++){
        *(pola + i) = calloc(sizeof(wchar_t) , kolumny);

        if(*(pola + i) == NULL){

            for (int j = 0; j < i; ++j) { //zwalnianie wierszy
                free(*(pola + j));
            }

            free(pola);
            free(nowa_plansza);

            return NULL;

        }

    }
	for (int i = 0; i < wiersze; ++i) {
        	for (int j = 0; j < kolumny; ++j) {
            	pola[i][j]=BIALY;
        	}	
    	}
    nowa_plansza->pola = pola;

    return nowa_plansza;

}

void wyswietl(struct plansza* plansza, struct mrowka* mrowka){
	
	wprintf(L"%lc", ROG_GORA_LEWO);
	for (int x = 0; x < plansza->kolumny; ++x){
		wprintf(L"%lc", RAMKA_POZIOMA);
	}

	wprintf(L"%lc", ROG_GORA_PRAWO);
	wprintf(L"%lc", '\n');

    for (int y = 0; y < plansza->wiersze; ++y) {
	    wprintf(L"%lc", RAMKA_PIONOWA);

        for (int x = 0; x < plansza->kolumny + 1; ++x) {
            if(x == mrowka->x && y == mrowka->y){
                wprintf(L"%lc", mrowka->kierunek);
            }else if (x == plansza->kolumny){
	    	wprintf(L"%lc", RAMKA_PIONOWA);
	    }else{
                wprintf(L"%lc", plansza->pola[x][y]);
            }
        }
        wprintf(L"%lc", '\n');
    }

    wprintf(L"%lc", ROG_DOL_LEWO);
    for (int x = 0; x < plansza->kolumny; ++x){
                wprintf(L"%lc", RAMKA_POZIOMA);
        }
    wprintf(L"%lc", ROG_DOL_PRAWO);
    wprintf(L"%lc", '\n');
}


wchar_t obrot_prawo(wchar_t kierunek){

    if(kierunek == LEWO){
        return GORA;
    } else if(kierunek == GORA){
        return PRAWO;
    } else if (kierunek == PRAWO){
        return DOL;
    } else{
        return LEWO;
    }
}


wchar_t obrot_lewo(wchar_t kierunek){

    if(kierunek == LEWO){
        return DOL;
    } else if(kierunek == GORA){
        return LEWO;
    } else if (kierunek == PRAWO){
        return GORA;
    } else{
        return PRAWO;
    }
}

int odwrocenie_koloru(int kolor){
    if(kolor == BIALY){
        return CZARNY;
    }else{
        return BIALY;
    }
}

void przesuniecie_mrowki(struct mrowka* mrowka){
    if(mrowka->kierunek == GORA){
        mrowka->y--;
    }else if (mrowka->kierunek == LEWO){
        mrowka->x--;
    }else if (mrowka->kierunek == DOL){
        mrowka->y++;
    }else{
        mrowka->x++;
    }
}

void poruszanie_biale(struct plansza* plansza, struct mrowka* mrowka){

    mrowka->kierunek = obrot_prawo(mrowka->kierunek);

    plansza->pola[mrowka->x][mrowka->y] = odwrocenie_koloru(plansza->pola[mrowka->x][mrowka->y]);

    przesuniecie_mrowki(mrowka);

}

void poruszanie_czarne(struct plansza* plansza, struct mrowka* mrowka){

    mrowka->kierunek = obrot_lewo(mrowka->kierunek);

    plansza->pola[mrowka->x][mrowka->y] = odwrocenie_koloru(plansza->pola[mrowka->x][mrowka->y]);

    przesuniecie_mrowki(mrowka);

}

void poruszanie(struct plansza* plansza, struct mrowka* mrowka){

    if (plansza->pola[mrowka->x][mrowka->y] == BIALY){
        poruszanie_biale(plansza,mrowka);
    }else{
        poruszanie_czarne(plansza,mrowka);
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
    setlocale(LC_CTYPE, "");


    struct plansza* plansza = tworzenie_planszy(10,10);
    struct mrowka* mrowka = tworzenie_morowki(5, 5, GORA);

    wyswietl(plansza, mrowka);
    for (int i = 0; i < 3; ++i) {
        poruszanie(plansza, mrowka);
        wyswietl(plansza, mrowka);
    }


    zwolnij_plansze(plansza);
    return 0;
}
