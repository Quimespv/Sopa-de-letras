#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Definició de l'orientació de les paraules
typedef enum {
    HORITZONTAL,
    VERTICAL,
    DIAGONAL
} tOrientation;

// Definició de la posició d'una paraula
typedef struct {
    int x;
    int y;
    tOrientation orientacio;
} tPosition;

// Definició d'una paraula dins la sopa de lletres
typedef struct {
    char text[16];
    int trobada;
    tPosition posicio;
} tWord;

// Definició de la sopa de lletres
typedef enum {
    BUIDA,
    ALEATORIA,
    COMPLETA
} tSoupState;

typedef struct {
    char grid[50][50];
    int files;
    int columnes;
    tSoupState estat;
} tSoup;

// Definició de la llista de paraules a buscar
typedef struct {
    tWord paraules[15];
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
tWord readWord();
void reverseWord(char *word); //afegim reverseWord
char getRandomCharacter();
void fillWithRandomCharacters(int n, int m, tSoup *soup);
void writeSoup(int n, int m, tSoup *soup);
void hideWordIntoSoup(tSearch *search, int wordIndex, int n, int m, tSoup *soup);
void addWordToSearch(tSearch *search, tWord word);
void initSearch(tSearch *search);
void initSoup(int n, int m, tSoup *soup);

// Implementació de la funció que inverteix una paraula
void reverseWord(char *word) {
    int len = strlen(word);
    int i;
    for (i = 0; i < len / 2; i++) {
        char temp = word[i];
        word[i] = word[len - i - 1];
        word[len - i - 1] = temp;
    }
}

// Implementació de funcions
char getRandomCharacter() {
    return 'A' + (rand() % 26);
}

void fillWithRandomCharacters(int n, int m, tSoup *soup) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            soup->grid[i][j] = getRandomCharacter();
        }
    }
}

void writeSoup(int n, int m, tSoup *soup) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%c ", soup->grid[i][j]);
        }
        printf("\n");
    }
}

void hideWordIntoSoup(tSearch *search, int wordIndex, int n, int m, tSoup *soup) {
    tWord *word = &search->paraules[wordIndex];
    int len = strlen(word->text);
    int ori = rand() % 3;
    int reverse = rand() % 2;
    int i, j, k;

    if (reverse) {
        reverseWord(word->text);
    }

    if (ori == 0) {
        i = rand() % (n - len + 1);
        j = rand() % m;
        for (k = 0; k < len; k++) {
            soup->grid[i + k][j] = word->text[k];
        }
    } else if (ori == 1) {
        i = rand() % n;
        j = rand() % (m - len + 1);
        for (k = 0; k < len; k++) {
            soup->grid[i][j + k] = word->text[k];
        }
    } else {
        i = rand() % (n - len + 1);
        j = rand() % (m - len + 1);
        for (k = 0; k < len; k++) {
            soup->grid[i + k][j + k] = word->text[k];
        }
    }
}

void addWordToSearch(tSearch *search, tWord word) {
    if (search->numParaules < 15) {
        search->paraules[search->numParaules++] = word;
    }
}

void initSearch(tSearch *search) {
    search->numParaules = 0;
}

void initSoup(int n, int m, tSoup *soup) {
    soup->files = n;
    soup->columnes = m;
    soup->estat = ALEATORIA;
}

int main() {
    srand(time(NULL));
    tGame partida;
    int n, m, w, i;
    
    printf("Introdueix dimensions de la sopa (N M): ");
    scanf("%d %d", &n, &m);
    
    initSoup(n, m, &partida.sopa);
    initSearch(&partida.cerca);
    fillWithRandomCharacters(n, m, &partida.sopa);
    
    printf("Introdueix el nombre de paraules: ");
    scanf("%d", &w);
    
    for (i = 0; i < w; i++) {
        tWord word;
        printf("Introdueix la paraula %d: ", i + 1);
        scanf("%s", word.text);
        addWordToSearch(&partida.cerca, word);
        hideWordIntoSoup(&partida.cerca, i, n, m, &partida.sopa);
    }
    
    writeSoup(n, m, &partida.sopa);
    return 0;
}
