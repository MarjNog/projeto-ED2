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
        printf("%d) %s\n", i+1, filmes[i].originalTitle);
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

int compareOriginalTitle (const void *a, const void *b) {
    return strcmp (((struct stFilm *)a)->originalTitle,((struct stFilm *)b)->originalTitle);
}

int main (int ac, char **av){
    int const max = 10000;
    tpFilm film;
    tpFilm ultimo;
    int menorIdx = -1;

    tpFilm *vet = malloc(max * sizeof(tpFilm));
    tpFilm *reservatorio = malloc(max * sizeof(tpFilm));
    int countM = 0;
    int countR = 0;

    int countTest = 0;//debug


    //file handling
    FILE *arquivo_entrada = fopen("films.dat", "rb");
    int countSaida = 1;
    char saidaNome[15];
    sprintf(saidaNome, "p%d.dat", countSaida);
    FILE *arquivo_saida = fopen(saidaNome, "wb");
    assert(arquivo_entrada != NULL);

    while ((fread(&film, sizeof(tpFilm), 1, arquivo_entrada) > 0)){
        if (countM < max){//inicia o vetor com MAX
            vet[countM] = film;
            countM++;
            continue;
        }
        //se o proximo for menor que o menor atual, mandar pro reservatorio, repetir
                            //-1 se primeiro arg menor que segundo
        if(menorIdx >= 0 && strcmp(film.originalTitle, ultimo.originalTitle) < 0){
            if(countR > max){//se reservatorio cheio
                qsort(vet, max, sizeof(tpFilm), compareOriginalTitle);
                fwrite(vet, sizeof(vet), 1, arquivo_saida);
                memcpy(vet, reservatorio, sizeof(tpFilm)*max);
                countR = 0;

                fclose(arquivo_saida);
                countSaida++;
                sprintf(saidaNome, "p%d.dat", countSaida);
                arquivo_saida = fopen(saidaNome, "wb");
            }
            reservatorio[countR] = film;
            countR++;
            if (countR%10000==0){
                printf("reservatorio cheio\n");
            }

        } else{//finds min
            menorIdx = findMinIdx(vet,max);
            fwrite (&vet[menorIdx], sizeof(tpFilm), 1, arquivo_saida);
            ultimo = vet[menorIdx];
            vet[menorIdx] = film;
        }
    }

    //se reservtorio n estiver vazio
    if (countR != 0){
        fclose(arquivo_saida);
        countSaida++;
        sprintf(saidaNome, "p%d.dat", countSaida);
        arquivo_saida = fopen(saidaNome, "wb");
        qsort(vet, countR, sizeof(tpFilm), compareOriginalTitle);
        fwrite(reservatorio, sizeof(tpFilm), countR, arquivo_saida);
    }

    fclose(arquivo_saida);
    fclose(arquivo_entrada);

    //intercalaçao otima F=3
    int const totalSaida = countsaida;
    int countEntrada=1;
    countSaida++;
    char EntradaNome1[15];
    char EntradaNome2[15];
    sprintf(EntradaNome1, "p%d.dat", countSaida);
    countSaida++;
    sprintf(EntradaNome2, "p%d.dat", countSaida);
    sprintf(EntradaNome2, "p%d.dat", countSaida);

    return 0;
}
