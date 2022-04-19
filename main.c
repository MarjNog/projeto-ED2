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

void escreveTudo(FILE *fileIn, FILE *fileOut){
    tpFilm buffer;
    while ((fread(&buffer, sizeof(tpFilm), 1, fileIn) > 0)){
        fwrite (&buffer, sizeof(tpFilm), 1, fileOut);
    }
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
                for(int i = 0; i < max; i++){
                 fwrite(&vet[i], sizeof(tpFilm), 1, arquivo_saida);
                }
                memcpy(vet, reservatorio, sizeof(tpFilm)*max);
                countR = 0;

                fclose(arquivo_saida);
                countSaida++;
                sprintf(saidaNome, "p%d.dat", countSaida);
                arquivo_saida = fopen(saidaNome, "wb");
                printf("particao %d\n", countSaida);
            }
            reservatorio[countR] = film;
            countR++;

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

    //intercalacao otima F=3
    int iIn = 1;
    char entrada1[10];
    char entrada2[10];
    sprintf(entrada1, "p%d.dat", iIn);
    iIn++;
    sprintf(entrada2, "p%d.dat", iIn);

    FILE *fileIn1 = fopen(entrada1, "rb");
    FILE *fileIn2 = fopen(entrada2, "rb");

    int totalP=countSaida;
    totalP++;
    char saida[10];
    sprintf(saida, "p%d.dat", totalP);
    FILE *fileOut = fopen(saida, "wb");

    tpFilm film1;
    tpFilm film2;

    int primeiro = 1;

    while(iIn < totalP){
        if(primeiro > 0){
            fread(&film1, sizeof(tpFilm), 1, fileIn1);
            fread(&film2, sizeof(tpFilm), 1, fileIn2);
            primeiro = 0;
        }
                    //-1 se primeiro arg menor que segundo
        if(strcmp(film1.originalTitle, film2.originalTitle) < 0){
            fwrite (&film1, sizeof(tpFilm), 1, fileOut);
            if (!(fread(&film1, sizeof(tpFilm), 1, fileIn1) > 0)){//se fim de arquivo1, escreve arqv2
                printf("fim arqv1\n");
                escreveTudo(fileIn2, fileOut);

                fclose(fileIn1);
                fclose(fileIn2);
                fclose(fileOut);

                iIn++;
                sprintf(entrada1, "p%d.dat", iIn);
                iIn++;
                sprintf(entrada2, "p%d.dat", iIn);
                fileIn1 = fopen(entrada1, "rb");
                fileIn2 = fopen(entrada2, "rb");
                totalP++;
                if(iIn < totalP){
                    sprintf(saida, "p%d.dat", totalP);
                    fileOut = fopen(saida, "wb");
                }

            }
        } else {
            fwrite (&film2, sizeof(tpFilm), 1, fileOut);
            if (!(fread(&film2, sizeof(tpFilm), 1, fileIn2) > 0)){//se fim de arquivo2, escreve arqv1
                printf("fim arqv2\n");
                escreveTudo(fileIn1, fileOut);

                fclose(fileIn1);
                fclose(fileIn2);
                fclose(fileOut);

                iIn++;
                sprintf(entrada1, "p%d.dat", iIn);
                iIn++;
                sprintf(entrada2, "p%d.dat", iIn);
                fileIn1 = fopen(entrada1, "rb");
                fileIn2 = fopen(entrada2, "rb");
                totalP++;
                if(iIn < totalP){
                    sprintf(saida, "p%d.dat", totalP);
                    fileOut = fopen(saida, "wb");
                }
            }
        }

    }

    return 0;
}
