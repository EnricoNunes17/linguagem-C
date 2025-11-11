#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "feifood.h"

void avaliar_pedido(const char *username) {
    printf("\n--- Avaliar Pedido ---\n");
    
    
    char nomeAlimento[50];
    int nota;

    printf("Digite o nome do alimento que deseja avaliar: ");
    fgets(nomeAlimento, sizeof(nomeAlimento), stdin);
    nomeAlimento[strcspn(nomeAlimento, "\n")] = 0;

    Alimento al = buscar_alimento_por_nome(nomeAlimento);
    if (al.preco == 0) {
        printf("Alimento '%s' não encontrado no cardápio.\n", nomeAlimento);
        return;
    }

  
    printf("Digite sua nota (0 a 5 estrelas) para %s: ", nomeAlimento);
    scanf("%d", &nota);
    getchar();

    if (nota < 0 || nota > 5) {
        printf("Nota inválida. Deve ser entre 0 e 5.\n");
        return;
    }

    
    FILE *arq = fopen("avaliacoes.txt", "a");
    if (!arq) {
        printf("Erro ao salvar avaliação!\n");
        return;
    }

    fprintf(arq, "%s,%s,%d\n", username, nomeAlimento, nota);
    fclose(arq);

    printf("Avaliação registrada com sucesso! Obrigado!\n");
}