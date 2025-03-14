#include <stdio.h>

// Definició de l'orientació de les paraules
typedef enum {
    HORITZONTAL,
    VERTICAL,
    DIAGONAL
} tOrientation;

// Definició de la posició d'una paraula
typedef struct {
    int x; // Coordenada horitzontal
    int y; // Coordenada vertical
    tOrientation orientacio;
} tPosition;

// Definició d'una paraula dins la sopa de lletres
typedef struct {
    char text[16]; // Fins a 15 caràcters + '\0'
    int trobada;   // 1 si ha estat trobada, 0 si no
    tPosition posicio;
} tWord;

// Definició de la sopa de lletres
typedef enum {
    BUIDA,
    ALEATORIA,
    COMPLETA
} tSoupState;

typedef struct {
    char grid[50][50]; // Mida màxima de 50x50
    int files;
    int columnes;
    tSoupState estat;
} tSoup;

// Definició de la llista de paraules a buscar
typedef struct {
    tWord paraules[15]; // Fins a 15 paraules
    int numParaules;
} tSearch;

// Definició del joc de sopa de lletres
typedef struct {
    tSoup sopa;
    tSearch cerca;
} tGame;

// Declaració de funcions
char getCharacter(tSoup sopa, int fila, int columna);
void setCharacter(tSoup *sopa, int fila, int columna, char caracter);
void setWord(tSoup *sopa, tWord *paraula, tPosition posicio, tOrientation orientacio);
void initSoup(tSoup *sopa, int files, int columnes);
tWord readWord();

int main() {
    // Exemple d'ús de les estructures
    tGame partida;
    partida.sopa.files = 15;
    partida.sopa.columnes = 15;
    partida.sopa.estat = BUIDA;
    
    printf("Sopa de lletres inicialitzada amb %d files i %d columnes.\n", partida.sopa.files, partida.sopa.columnes);
    return 0;
}

