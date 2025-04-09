#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// Definicio de la sopa de lletres
typedef struct {
    char grid[50][50];
    int files;
    int columnes;
} tSoup;

// Funcions per crear la sopa
char getRandomCharacter() {
    return 'A' + (rand() % 26);
}

void fillWithRandomCharacters(tSoup *soup) {
    int i, j;
    for (i = 0; i < soup->files; i++) {
        for (j = 0; j < soup->columnes; j++) {
            soup->grid[i][j] = getRandomCharacter();
        }
    }
}

void hideWord(tSoup *soup, char *word) {
    int len = strlen(word);
    int ori = rand() % 3; // 0: Horizontal, 1: Vertical, 2: Diagonal
    int reverse = rand() % 2;
    int i, j, k;
    char *tempWord = strdup(word);
    
    // Invertir la paraula si es necessari
    if (reverse) {
        int l;
        for (l = 0; l < len / 2; l++) {
            char temp = tempWord[l];
            tempWord[l] = tempWord[len - l - 1];
            tempWord[len - l - 1] = temp;
        }
    }
    
    if (ori == 0) { // Horizontal
        i = rand() % soup->files;
        j = rand() % (soup->columnes - len + 1);
        for (k = 0; k < len; k++) {
            soup->grid[i][j + k] = tempWord[k];
        }
    } else if (ori == 1) { // Vertical
        i = rand() % (soup->files - len + 1);
        j = rand() % soup->columnes;
        for (k = 0; k < len; k++) {
            soup->grid[i + k][j] = tempWord[k];
        }
    } else { // Diagonal
        i = rand() % (soup->files - len + 1);
        j = rand() % (soup->columnes - len + 1);
        for (k = 0; k < len; k++) {
            soup->grid[i + k][j + k] = tempWord[k];
        }
    }
    
    free(tempWord);
}

void writeSoup(tSoup soup) {
    int i, j;
    for (i = 0; i < soup.files; i++) {
        for (j = 0; j < soup.columnes; j++) {
            printf("%c ", soup.grid[i][j]);
        }
        printf("\n");
    }
}

// Funcions per resoldre la sopa
int isUpperCaseLetter(char c) {
    return c >= 'A' && c <= 'Z';
}

void initOutputSoup(tSoup *soup, tSoup *outputSoup) {
    int i, j;
    
    outputSoup->files = soup->files;
    outputSoup->columnes = soup->columnes;
    
    for (i = 0; i < soup->files; i++) {
        for (j = 0; j < soup->columnes; j++) {
            outputSoup->grid[i][j] = '.';
        }
    }
}

void checkWord(tSoup soup, char *word, int x, int y, int dirX, int dirY, int *found) {
    int i;
    int match = 1;
    int inBounds = (x >= 0) && (x < soup.files) && (y >= 0) && (y < soup.columnes);
    int currX = x, currY = y;
    
    i = 0;
    while (i < strlen(word) && inBounds && match) {
        match = (word[i] == soup.grid[currX][currY]);
        currX += dirX;
        currY += dirY;
        inBounds = (currX >= 0) && (currX < soup.files) && (currY >= 0) && (currY < soup.columnes);
        i++;
    }
    
    *found = match && (i == strlen(word));
}

void lookForWord(tSoup soup, char *word, int *startX, int *startY, int *dirX, int *dirY, int *found) {
    int x, y;
    int dx, dy;
    
    *found = 0;
    
    for (x = 0; x < soup.files && !(*found); x++) {
        for (y = 0; y < soup.columnes && !(*found); y++) {
            for (dx = -1; dx <= 1 && !(*found); dx++) {
                for (dy = -1; dy <= 1 && !(*found); dy++) {
                    // Evitar direccio (0,0)
                    if (dx == 0 && dy == 0) continue;
                    
                    checkWord(soup, word, x, y, dx, dy, found);
                    if (*found) {
                        *startX = x;
                        *startY = y;
                        *dirX = dx;
                        *dirY = dy;
                        return;
                    }
                }
            }
        }
    }
}

void putWord(tSoup *soup, char *word, int startX, int startY, int dirX, int dirY) {
    int i;
    int x = startX, y = startY;
    
    for (i = 0; i < strlen(word); i++) {
        soup->grid[x][y] = word[i];
        x += dirX;
        y += dirY;
    }
}

void writeOutputSoup(tSoup soup) {
    int i, j;
    
    printf("\nSolucio:\n");
    printf("%d %d\n", soup.files, soup.columnes);
    
    for (i = 0; i < soup.files; i++) {
        for (j = 0; j < soup.columnes; j++) {
            printf("%c ", soup.grid[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    tSoup soup, outputSoup;
    int numWords, i, j;
    char word[16];
    
    // Crear la sopa de lletres
    printf("Introdueix dimensions de la sopa (N M): ");
    scanf("%d %d", &soup.files, &soup.columnes);
    
    // Inicialitzar la sopa amb lletres aleatòries
    fillWithRandomCharacters(&soup);
    
    // Demanar paraules per amagar
    printf("Introdueix el nombre de paraules a amagar: ");
    scanf("%d", &numWords);
    
    for (i = 0; i < numWords; i++) {
        printf("Introdueix la paraula %d: ", i + 1);
        scanf("%s", word);
        
        // Convertir a majúscules
        for (j = 0; j < strlen(word); j++) {
            word[j] = toupper(word[j]);
        }
        
        hideWord(&soup, word);
    }
    
    // Mostrar la sopa de lletres
    printf("\nSopa de lletres:\n");
    writeSoup(soup);
    
    // Inicialitzar la sopa de sortida
    initOutputSoup(&soup, &outputSoup);
    
    // Demanar paraules per buscar
    printf("\nIntrodueix el nombre de paraules a buscar: ");
    scanf("%d", &numWords);
    
    for (i = 0; i < numWords; i++) {
        int startX, startY, dirX, dirY, found;
        
        printf("Introdueix la paraula %d a buscar: ", i + 1);
        scanf("%s", word);
        
        // Convertir a majúscules
        for (j = 0; j < strlen(word); j++) {
            word[j] = toupper(word[j]);
        }
        
        // Buscar la paraula
        lookForWord(soup, word, &startX, &startY, &dirX, &dirY, &found);
        
        if (found) {
            printf("Paraula '%s' trobada\n", word);
            putWord(&outputSoup, word, startX, startY, dirX, dirY);
        } else {
            printf("Paraula '%s' no trobada.\n", word);
        }
    }
    
    // Mostrar la solució
    writeOutputSoup(outputSoup);
    
    return 0;
}
