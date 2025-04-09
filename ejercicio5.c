#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

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

// Declaració de funcions originals
char getCharacter(tSoup sopa, int fila, int columna);
void setCharacter(tSoup *sopa, int fila, int columna, char caracter);
void setWord(tSoup *sopa, tWord *paraula, tPosition posicio, tOrientation orientacio);
tWord readWord();
void reverseWord(char *word);
char getRandomCharacter();
void fillWithRandomCharacters(int n, int m, tSoup *soup);
void writeSoup(int n, int m, tSoup *soup);
void hideWordIntoSoup(tSearch *search, int wordIndex, int n, int m, tSoup *soup);
void addWordToSearch(tSearch *search, tWord word);
void initSearch(tSearch *search);
void initSoup(int n, int m, tSoup *soup);

// Declaració de funcions per a l'exercici 5
void readInputSoup(tSoup *soup);
void initOutputSoup(tSoup *soup);
int isUpperCaseLetter(char c);
tWord readString();
void lookForWord(tSoup inputSoup, tWord word, int *x, int *y, int *dirX, int *dirY, int *found);
void checkWord(tSoup soup, tWord currentWord, int x, int y, int dirX, int dirY, int *found);
void putWord(tSoup *soup, tWord currentWord, int x, int y, int dirX, int dirY);
void writeSoupEx5(tSoup soup);

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

// Implementació de funcions originals
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

// Implementació de funcions per a l'exercici 5
void readInputSoup(tSoup *soup) {
    int i, j;
    char c;
    
    for (i = 0; i < soup->files; i++) {
        for (j = 0; j < soup->columnes; j++) {
            do {
                scanf(" %c", &c);
            } while (!isUpperCaseLetter(c));
            soup->grid[i][j] = c;
        }
    }
}

void initOutputSoup(tSoup *soup) {
    int i, j;
    
    for (i = 0; i < soup->files; i++) {
        for (j = 0; j < soup->columnes; j++) {
            soup->grid[i][j] = '.';
        }
    }
}

int isUpperCaseLetter(char c) {
    return c >= 'A' && c <= 'Z';
}

tWord readString() {
    tWord string;
    char c;
    int length = 0;
    
    do {
        scanf(" %c", &c);
    } while (!isUpperCaseLetter(c));
    
    string.text[0] = '\0';
    length = 0;
    
    while (isUpperCaseLetter(c)) {
        string.text[length++] = c;
        string.text[length] = '\0';
        if (scanf("%c", &c) != 1) break;
    }
    
    return string;
}

void lookForWord(tSoup inputSoup, tWord word, int *x, int *y, int *dirX, int *dirY, int *found) {
    *found = 0;
    
    for (*dirX = -1; *dirX <= 1 && !(*found); (*dirX)++) {
        for (*dirY = -1; *dirY <= 1 && !(*found); (*dirY)++) {
            // Evitar direcció (0,0)
            if (*dirX == 0 && *dirY == 0) continue;
            
            checkWord(inputSoup, word, *x, *y, *dirX, *dirY, found);
        }
    }
    
    (*dirX)--;
    (*dirY)--;
}

void checkWord(tSoup soup, tWord currentWord, int x, int y, int dirX, int dirY, int *found) {
    int i;
    int match = 1;
    int inBounds = (x >= 0) && (x < soup.files) && (y >= 0) && (y < soup.columnes);
    int currX = x, currY = y;
    
    i = 0;
    while (i < strlen(currentWord.text) && inBounds && match) {
        match = (currentWord.text[i] == soup.grid[currX][currY]);
        currX += dirX;
        currY += dirY;
        inBounds = (currX >= 0) && (currX < soup.files) && (currY >= 0) && (currY < soup.columnes);
        i++;
    }
    
    *found = match && (i == strlen(currentWord.text));
}

void putWord(tSoup *soup, tWord currentWord, int x, int y, int dirX, int dirY) {
    int i;
    int currX = x, currY = y;
    
    for (i = 0; i < strlen(currentWord.text); i++) {
        soup->grid[currX][currY] = currentWord.text[i];
        currX += dirX;
        currY += dirY;
    }
}

void writeSoupEx5(tSoup soup) {
    int i, j;
    
    printf("%d %d\n", soup.files, soup.columnes);
    
    for (i = 0; i < soup.files; i++) {
        for (j = 0; j < soup.columnes; j++) {
            printf("%c ", soup.grid[i][j]);
        }
        printf("\n");
    }
}

// Funció principal per a l'exercici 5
int letterSoup() {
    int i, w, x, y, dirX, dirY;
    tSoup inputSoup, outputSoup;
    tWord currentWord;
    int found;
    
    scanf("%d %d", &inputSoup.files, &inputSoup.columnes);
    
    readInputSoup(&inputSoup);
    
    outputSoup.files = inputSoup.files;
    outputSoup.columnes = inputSoup.columnes;
    
    initOutputSoup(&outputSoup);
    
    scanf("%d", &w);
    
    for (i = 0; i < w; i++) {
        currentWord = readString();
        found = 0;
        
        for (x = 0; x < inputSoup.files && !found; x++) {
            for (y = 0; y < inputSoup.columnes && !found; y++) {
                lookForWord(inputSoup, currentWord, &x, &y, &dirX, &dirY, &found);
            }
        }
        
        if (found) {
            putWord(&outputSoup, currentWord, x-1, y-1, dirX, dirY);
        }
    }
    
    writeSoupEx5(outputSoup);
    
    return 0;
}

int main() {
    srand(time(NULL));
    int opcio;
    
    printf("Selecciona una opció:\n");
    printf("1. Crear sopa de lletres\n");
    printf("2. Resoldre sopa de lletres (exercici 5)\n");
    scanf("%d", &opcio);
    
    if (opcio == 1) {
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
    } else if (opcio == 2) {
        letterSoup();
    } else {
        printf("Opció no vàlida.\n");
    }
    
    return 0;
}
