#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "feifood.h"



void str_tolower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void buscar_alimento() {
    FILE *arq = fopen("alimentos.txt", "r");
    if (!arq) {
        printf("Arquivo de alimentos não encontrado!\n");
        return;
    }

    char busca[50];
    printf("Digite o tipo ou nome do alimento que deseja buscar: ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = 0; 

    char buscaLower[50];
    strcpy(buscaLower, busca);
    str_tolower(buscaLower);

    char linha[150];
    int encontrados = 0;

    printf("\nResultados da busca por '%s':\n", busca);

    while (fgets(linha, sizeof(linha), arq)) {
        char nome[50], categoria[50];
        int calorias;
        double preco;

        sscanf(linha, "%[^,],%[^,],%d,%lf", nome, categoria, &calorias, &preco);

       
        char nomeLower[50], categoriaLower[50];
        strcpy(nomeLower, nome);
        strcpy(categoriaLower, categoria);
        str_tolower(nomeLower);
        str_tolower(categoriaLower);

        
        if (strstr(nomeLower, buscaLower) || strstr(categoriaLower, buscaLower)) {
            printf(" - %s (%s)\n", nome, categoria);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum alimento encontrado com o termo '%s'.\n", busca);
    }

    fclose(arq);
}
void listar_info_alimento() {
    FILE *arq = fopen("alimentos.txt", "r");
    if (!arq) {
        printf("Arquivo de alimentos não encontrado!\n");
        return;
    }

    char nomeBusca[50];
    printf("Digite o nome exato do alimento para ver detalhes: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    char linha[150];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), arq)) {
        char nome[50], categoria[50];
        int calorias;
        double preco;

        sscanf(linha, "%[^,],%[^,],%d,%lf", nome, categoria, &calorias, &preco);

        if (strcasecmp(nome, nomeBusca) == 0) {
            printf("\nInformações do alimento:\n");
            printf("Nome: %s\n", nome);
            printf("Categoria: %s\n", categoria);
            printf("Calorias: %d kcal\n", calorias);
            printf("Preço: R$ %.2f\n", preco);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Alimento '%s' não encontrado!\n", nomeBusca);
    }

    fclose(arq);
}
Alimento buscar_alimento_por_nome(const char *nomeBusca) {
    Alimento al = {0};
    FILE *arq = fopen("alimentos.txt", "r");
    if (!arq) {
        return al; 
    }

    char linha[150];
    while (fgets(linha, sizeof(linha), arq)) {
        Alimento temp;
        sscanf(linha, "%[^,],%[^,],%d,%lf", temp.nome, temp.categoria, &temp.calorias, &temp.preco);

        if (strcasecmp(temp.nome, nomeBusca) == 0) {
            fclose(arq);
            return temp; 
        }
    }
    fclose(arq);
    return al; 
}