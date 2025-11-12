#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "feifood.h"


void menu_principal() {
    int opcao;
    do {
        printf("\n==== FEIFood ====\n");
        printf("1 - Cadastrar novo usuario\n");
        printf("2 - Login de usuario\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); 

        switch(opcao) {
            case 1: cadastrar_usuario(); break;
            case 2: login_usuario(); break;
            case 0: printf("Saindo...\n"); exit(0);
            default: printf("Opcao invalida!\n");
        }
    } while (1);
}

void cadastrar_usuario() {
    FILE *arq = fopen("usuarios.txt", "a");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo usuarios.txt\n");
        return;
    }

    char nome[50], email[50], senha[20];

    printf("Nome: ");
    scanf("%49s", nome); 
    printf("Email: ");
    scanf("%49s", email);
    printf("Senha: ");
    scanf("%19s", senha);

    fprintf(arq, "%s,%s,%s\n", nome, email, senha);
    fclose(arq);
    printf("Usuário cadastrado com sucesso!\n");
}

void login_usuario() {
    FILE *arq = fopen("usuarios.txt", "r");
    if (arq == NULL) { 
        printf("Nenhum usuário cadastrado.\n"); 
        return; 
    }

    char email[50], senha[20];
    char linha[150];
    char nomeLido[50], emailLido[50], senhaLida[20];
    int encontrado = 0;

    printf("Email: ");
    scanf("%49s", email);
    printf("Senha: ");
    scanf("%19s", senha);
    getchar(); 

    while (fgets(linha, sizeof(linha), arq)) {
        
        sscanf(linha, "%[^,],%[^,],%s", nomeLido, emailLido, senhaLida); 
        if (strcmp(email, emailLido) == 0 && strcmp(senha, senhaLida) == 0) {
            printf("\nLogin bem-sucedido! Bem-vindo, %s.\n", nomeLido);
            encontrado = 1;
            fclose(arq);
            menu_usuario(nomeLido);
            return; 
        }
    }

    fclose(arq);
    if (!encontrado) {
        printf("Email ou senha incorretos!\n");
    }
}




void menu_usuario(const char *username) {
    int opcao;

    do {
        printf("\n=== Menu do Usuário (%s) ===\n", username);
        printf("1 - Buscar alimento\n");
        printf("2 - Listar informações de alimento\n");
        printf("3 - Gerenciar meus pedidos\n"); 
        printf("4 - Avaliar um item\n");      
        printf("0 - Logout\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                buscar_alimento();
                break;
            case 2:
                listar_info_alimento();
                break;
            case 3:
                menu_gerenciar_pedidos(username);
                break;
            case 4:
                avaliar_pedido(username);
                break;
            case 0:
                printf("Saindo do menu do usuário...\n");
                return;
            default:
                printf("Opção inválida!\n");
        }
    } while (1);
}