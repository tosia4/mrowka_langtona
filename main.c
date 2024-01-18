#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <getopt.h>


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

#define PRAWDA 1
#define FALSZ 0

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

void zwolnij_plansze(struct plansza* plansza){
    for (int i = 0; i < plansza->wiersze; ++i) {
        free(plansza->pola[i]);
    }
    free(plansza->pola);
    free(plansza);
}


struct plansza *wczytaj_plansze_z_pliku(const char *nazwa_pliku) {
    FILE *plik = fopen(nazwa_pliku, "r");

    if (plik == NULL) {
        fprintf(stderr, "Nie można otworzyć pliku %s\n", nazwa_pliku);
        return NULL;
    }

    int wiersze, kolumny;

    if (fscanf(plik, "%d %d", &wiersze, &kolumny) != 2) {
        fprintf(stderr, "Niepoprawny format pliku %s\n", nazwa_pliku);
        fclose(plik);
        return NULL;
    }

    struct plansza *wczytana_plansza = tworzenie_planszy(wiersze, kolumny);

    if (wczytana_plansza == NULL) {
        fprintf(stderr, "Błąd podczas alokacji pamięci dla planszy\n");
        fclose(plik);
        return NULL;
    }

    for (int i = 0; i < wiersze; ++i) {
        for (int j = 0; j < kolumny; ++j) {
	    wchar_t znak;
            if (fscanf(plik, " %lc", &znak) != 1) {
                fprintf(stderr, "Błąd podczas wczytywania planszy z pliku %s\n", nazwa_pliku);
                zwolnij_plansze(wczytana_plansza);

                fclose(plik);
                return NULL;
            }
	    wczytana_plansza->pola[i][j] = znak;
        }
    }

    fclose(plik);
    return wczytana_plansza;
}

void czytanieargumentów(int argc, char **argv, int *m, int *n, int *it, char **plikdozapisu, char **czytaniepliku, int *kier, int *procent_zapelnienia)
{
    int opcja;

    while ((opcja = getopt(argc, argv, "m:n:i:z:c:k:p:")) != -1)
    {
        switch (opcja)
        {
        case 'm':
            if (atoi(optarg) <= 0)
            {
                fprintf(stderr, "Niepoprawna wartość argumentu %s\n", optarg);
                exit(1);
            }
            *m = atoi(optarg);
            break;
        case 'n':
            if (atoi(optarg) <= 0)
            {
                fprintf(stderr, "Niepoprawna wartość argumentu %s\n", optarg);
                exit(1);
            }
            *n = atoi(optarg);
            break;
        case 'i':
            if (atoi(optarg) <= 0)
            {
                fprintf(stderr, "Niepoprawna wartość argumentu %s\n", optarg);
                exit(1);
            }
            *it = atoi(optarg);
            break;
        case 'p':
            if (atoi(optarg) < 0 || atoi(optarg) > 100)
            {
                fprintf(stderr, "Niepoprawna wartość argumentu %s\n", optarg);
                exit(1);
            }
            *procent_zapelnienia = atoi(optarg);
            break;
        case 'k':
            if (atoi(optarg) < 0 || atoi(optarg) > 3)
            {
                fprintf(stderr, "Niepoprawna wartość argumentu %s\n", optarg);
                exit(1);
            }
            *kier = atoi(optarg);
            break;
        case 'c':
            *czytaniepliku = malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(*czytaniepliku, optarg);
            break;
        case 'z':
            *plikdozapisu = malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(*plikdozapisu, optarg);
            break;
        case ':': /* brakuje argumentu związanego z opcją */
            fprintf(stderr, "opcja -%c wymaga argumentu\n", optopt);
            break;
        case '?':
            fprintf(stderr, "Niepoprawna opcja wywołania\n");
            break;
        }
    }
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
                wprintf(L"%lc", plansza->pola[y][x]);
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

    plansza->pola[mrowka->y][mrowka->x] = odwrocenie_koloru(plansza->pola[mrowka->y][mrowka->x]);

    przesuniecie_mrowki(mrowka);

}

void poruszanie_czarne(struct plansza* plansza, struct mrowka* mrowka){

    mrowka->kierunek = obrot_lewo(mrowka->kierunek);

    plansza->pola[mrowka->y][mrowka->x] = odwrocenie_koloru(plansza->pola[mrowka->y][mrowka->x]);

    przesuniecie_mrowki(mrowka);

}

void poruszanie(struct plansza* plansza, struct mrowka* mrowka){

    if (plansza->pola[mrowka->y][mrowka->x] == BIALY){
        poruszanie_biale(plansza,mrowka);
    }else{
        poruszanie_czarne(plansza,mrowka);
    }
}


int czy_czarne_pole(struct plansza* plansza, int kolumna, int wiersz){
    if(plansza->pola[wiersz][kolumna] == CZARNY){
        return PRAWDA;
    }else{
        return FALSZ;
    }
}

void losowanie_pol(struct plansza* plansza, int procent_zapelnienia){
    srand(time(NULL));
    int liczba_pol = plansza->wiersze*plansza->kolumny;

    int liczba_czarnych_pol = (((float)procent_zapelnienia/100) * (float)liczba_pol);


    for (int i = 0; i < liczba_czarnych_pol; ++i) {
        int kolumna;
        int wiersz;

        do{
            kolumna = rand()%plansza->kolumny;
            wiersz = rand()%plansza->wiersze;
        }while(czy_czarne_pole(plansza,kolumna, wiersz)==PRAWDA);

        plansza->pola[wiersz][kolumna] = CZARNY;

    }

}



void zapis_do_pliku(char* nazwa_pliku, struct plansza* plansza, struct mrowka* mrowka){

    FILE* plik;
    plik = fopen(nazwa_pliku,"w");

    fputwc(ROG_GORA_LEWO, plik);
    for (int x = 0; x < plansza->kolumny; ++x){
        fputwc(RAMKA_POZIOMA, plik);
    }

    fputwc(ROG_GORA_PRAWO, plik);
    fputwc(L'\n', plik);

    for (int y = 0; y < plansza->wiersze; ++y) {
        fputwc(RAMKA_PIONOWA, plik);

        for (int x = 0; x < plansza->kolumny + 1; ++x) {
            if(x == mrowka->x && y == mrowka->y){
                fputwc(mrowka->kierunek, plik);
            }else if (x == plansza->kolumny){
                fputwc(RAMKA_PIONOWA, plik);
            }else{
                fputwc(plansza->pola[y][x], plik);
            }
        }
        fputwc(L'\n', plik);
    }

    fputwc(ROG_DOL_LEWO, plik);
    for (int x = 0; x < plansza->kolumny; ++x){
        fputwc(RAMKA_POZIOMA,plik);
    }
    fputwc(ROG_DOL_PRAWO, plik);
    fputwc(L'\n', plik);
}


int main( int argc, char **argv) {
    setlocale(LC_CTYPE, "");
    int m = 0, n = 0, it = 0, procent_zapelnienia = -1, kier = 0;
    char *plikdozapisu = NULL;
    char *czytaniepliku = NULL;

    czytanieargumentów(argc, argv, &m, &n, &it, &plikdozapisu, &czytaniepliku, &kier, &procent_zapelnienia);
    //fprintf(stderr, "m=%d, n=%d, i=%d, proc=%d\n, plik=%c", m, n, it, procent_zapelnienia, plikdozapisu);
    
    struct plansza *plansza = NULL;

    if (czytaniepliku != NULL) {
        plansza = wczytaj_plansze_z_pliku(czytaniepliku);

        if (plansza == NULL) {
            fprintf(stderr, "Błąd podczas wczytywania planszy z pliku\n");
            free(plikdozapisu);
            free(czytaniepliku);
            return 1;
        }
    } else {
        // W przeciwnym razie utwórz nową planszę
        plansza = tworzenie_planszy(m, n);

        if (plansza == NULL) {
            fprintf(stderr, "Błąd podczas tworzenia planszy\n");
            free(plikdozapisu);
            free(czytaniepliku);
            return 1;
        }

        // Jeśli procent_zapelnienia >= 0, losuj pola
        if (procent_zapelnienia >= 0) {
            losowanie_pol(plansza, procent_zapelnienia);
        }
    }

    struct mrowka* mrowka;
    if(kier==0)mrowka = tworzenie_morowki(m/2, n/2, GORA);
    else if(kier==1)mrowka = tworzenie_morowki(m/2, n/2, DOL);
    else if(kier==2)mrowka = tworzenie_morowki(m/2, n/2, LEWO);
    else if(kier==3)mrowka = tworzenie_morowki(m/2, n/2, PRAWO);

    char numer_iteracji[4];

    wyswietl(plansza, mrowka);
    
    for (int i = 0; i < it; ++i) {
        sprintf(numer_iteracji, "%d", i); //zapisujemy nr iteracji jako string
	poruszanie(plansza, mrowka);

	//if(){
		wyswietl(plansza, mrowka);
	//} else {
		char filename[100]; //miejsce na nazwe pliku
        	memset(filename, 0, 100); //czyscimy pamiec
        	strcat(filename,"file_"); //sklejamy nazwe
        	strcat(filename, numer_iteracji); //sklejamy nazwe
        	zapis_do_pliku(filename, plansza, mrowka);
	//}
        
    }

    free(plikdozapisu);
    free(czytaniepliku);
    zwolnij_plansze(plansza);
    return 0;
}
