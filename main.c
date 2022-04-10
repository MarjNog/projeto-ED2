#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct stFilm{
  char  titleType[13],
        primaryTitle[422],
        originalTitle[422];
  unsigned char isAdult;
  unsigned short int startYear,
                      endYear;
  char runtimeMinutes[6],
       genres[33];
}tpFilm;

void printaFilmes(tpFilm *filmes, int tam){
    for (int i = 0; i < tam; i++){
        printf("%s\n", filmes[i].originalTitle);
    }
}

//retorna o I com o menor valor no array
int findMinIdx(tpFilm *filmes, int tam){
    char temp[422];
    int menorIdx = 0;

    strcpy(temp, filmes[0].originalTitle);
    for (int i = 0; i < tam; i++){
        //-1 se primeiro arg menor que segundo
        if(strcmp(filmes[i].originalTitle, temp) < 0){
            strcpy(temp, filmes[i].originalTitle);
            menorIdx = i;
        }
    }
    return menorIdx;
}

int main (int ac, char **av){
    int const max = 100;
    tpFilm film;
    int count = 0;
    tpFilm *vet = malloc(max * sizeof(tpFilm));
    //tpFilm *reservatorio = malloc(max * sizeof(tpFilm));

    int menorIdx = -1;
    char temp[422];
    int countTest = 0;

    //file handling
    FILE *arquivo_entrada = fopen("films.dat", "rb");
    int countSaida = 1;
    char saidaNome[15];
    sprintf(saidaNome, "p%d.dat", countSaida);
    FILE *arquivo_saida = fopen(saidaNome, "wb");
    assert(arquivo_entrada != NULL);


    while ((fread(&film, sizeof(tpFilm), 1, arquivo_entrada) > 0)){
        if (count < max){
            vet[count] = film;
            count++;
        }else{
            vet[menorIdx] = film;
        }

        //finds min
        menorIdx = findMinIdx(vet,max);
        strcpy(temp, vet[menorIdx].originalTitle);
        fwrite (&vet[menorIdx], sizeof(tpFilm), 1, arquivo_saida);
        countTest++;

    }

    //printaFilmes(vet,max);

    //printf("\n\n%s\n", vet[menorIdx].originalTitle);

    printf("\n\ntemp: %s", temp);

    fclose(arquivo_entrada);
    fclose(arquivo_saida);
    return EXIT_SUCCESS;
}
