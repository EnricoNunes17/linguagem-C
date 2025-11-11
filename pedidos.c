// pedidos.c (COMPLETO E CORRIGIDO - V2)

#include "feifood.h" // Inclui nossas structs (Alimento, Pedido) e outras funções

// ===================================================================
// MENU PRINCIPAL DE PEDIDOS
// ===================================================================

/**
 * @brief Exibe o menu de gerenciamento de pedidos (Criar, Listar, Excluir).
 * @param username O nome do usuário logado.
 */
void menu_gerenciar_pedidos(const char *username) {
    int opcao;
    do {
        printf("\n--- Gerenciar Pedidos de %s ---\n", username);
        printf("1 - Criar Novo Pedido\n");
        printf("2 - Listar Meus Pedidos Salvos\n");
        printf("3 - Excluir um Pedido Salvo\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do scanf

        switch(opcao) {
            case 1: criar_novo_pedido(username); break;
            case 2: listar_pedidos_usuario(username); break;
            case 3: excluir_pedido(username); break;
            case 0: return; // Volta ao menu anterior
            default: printf("Opcao invalida.\n");
        }
    } while(1);
}

// ===================================================================
// FUNÇÕES DE CRIAÇÃO DE PEDIDO (Ações 1, 2, 3 do PDF)
// ===================================================================

/**
 * @brief Inicia a criação de um novo pedido (carrinho de compras).
 * Permite adicionar, remover e ver itens antes de salvar.
 * @param username O nome do usuário que está criando o pedido.
 */
void criar_novo_pedido(const char *username) {
    // Aula 09: Declarando uma variável do tipo struct
    Pedido p;
    strcpy(p.username, username);
    p.num_itens = 0;
    p.total_preco = 0.0;

    int opcao;
    do {
        printf("\n--- Criando Novo Pedido ---\n");
        printf("Itens no carrinho: %d\n", p.num_itens);
        printf("Total: R$ %.2f\n", p.total_preco);
        printf("1 - Adicionar Alimento\n");
        printf("2 - Remover Alimento (último)\n");
        printf("3 - Ver Carrinho Detalhado\n");
        printf("4 - Finalizar e Salvar Pedido\n");
        printf("0 - Cancelar Pedido\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            // Aula 07: Passando o endereço da struct (&p)
            case 1: adicionar_item_pedido(&p); break;
            case 2: remover_item_pedido(&p); break;
            case 3: ver_carrinho(&p); break; 
            case 4: 
                salvar_pedido(&p);
                return; 
            case 0:
                printf("Pedido cancelado.\n");
                return;
            default: printf("Opcao invalida.\n");
        }
    } while (1);
}

/**
 * @brief Adiciona um alimento ao carrinho (struct Pedido).
 * @param p Ponteiro para a struct Pedido que está sendo modificada.
 */
void adicionar_item_pedido(Pedido *p) {
    if (p->num_itens >= 20) {
        printf("Carrinho cheio!\n");
        return;
    }

    char nomeAlimento[50];
    printf("Digite o nome exato do alimento para adicionar: ");
    fgets(nomeAlimento, sizeof(nomeAlimento), stdin);
    nomeAlimento[strcspn(nomeAlimento, "\n")] = 0;

    // Usa a função de 'alimentos.c' para buscar o alimento
    Alimento al = buscar_alimento_por_nome(nomeAlimento);

    if (al.preco == 0) { 
        printf("Alimento '%s' não encontrado!\n", nomeAlimento);
    } else {
        // Aula 07/09: Acessando membro de struct via ponteiro (->)
        // ou acessando array de struct direto (.)
        p->itens[p->num_itens] = al; 
        p->num_itens++;
        p->total_preco += al.preco; // Atualiza o total
        printf("%s (R$ %.2f) adicionado ao carrinho!\n", al.nome, al.preco);
    }
}

/**
 * @brief Remove o último alimento adicionado ao carrinho.
 * @param p Ponteiro para a struct Pedido.
 */
void remover_item_pedido(Pedido *p) {
    if (p->num_itens == 0) {
        printf("Carrinho já está vazio!\n");
        return;
    }
    
    p->num_itens--; // Apenas decrementa o contador
    Alimento al = p->itens[p->num_itens]; // Pega o item que foi "removido"
    p->total_preco -= al.preco; // Atualiza o total
    printf("Item %s (R$ %.2f) removido.\n", al.nome, al.preco);
}

/**
 * @brief Lista todos os itens e o total do carrinho atual.
 * @param p Ponteiro para a struct Pedido.
 */
void ver_carrinho(Pedido *p) {
    if (p->num_itens == 0) {
        printf("O carrinho está vazio.\n");
        return;
    }

    printf("\n--- Itens no Carrinho ---\n");
    
   
    for (int i = 0; i < p->num_itens; i++) {
        printf("Item %d: %s (R$ %.2f)\n", 
            i + 1, 
            p->itens[i].nome, 
            p->itens[i].preco);
    }
    printf("---------------------------\n");
    printf("Total de Itens: %d\n", p->num_itens);
    printf("Preço Total: R$ %.2f\n", p->total_preco);
}

/**
 * @brief Salva o pedido (carrinho) em "pedidos.txt".
 * @param p Ponteiro para a struct Pedido.
 */
void salvar_pedido(Pedido *p) {
    if (p->num_itens == 0) {
        printf("Não é possível salvar um pedido vazio.\n");
        return;
    }
    
    // Aula 08: fopen() no modo "a" (append)
    FILE *arq = fopen("pedidos.txt", "a"); 
    if (arq == NULL) {
        printf("Erro ao salvar pedido!\n");
        return;
    }

    
    fprintf(arq, "USER:%s,ITENS:%d,TOTAL:%.2f\n", 
        p->username, p->num_itens, p->total_preco);
    
    for (int i = 0; i < p->num_itens; i++) {
        fprintf(arq, "ITEM:%s,PRECO:%.2f\n", 
            p->itens[i].nome, p->itens[i].preco);
    }
    fprintf(arq, "---FIM---\n"); // Delimitador
    
    fclose(arq); // Aula 08: fclose()
    printf("Pedido salvo com sucesso!\n");
} // <--- **** ESTA CHAVE ESTAVA FALTANDO ****

// ===================================================================
// FUNÇÕES DE GERENCIAMENTO DE PEDIDOS SALVOS (Ação 4 do PDF)
// ===================================================================

/**
 * @brief Lista todos os pedidos salvos no "pedidos.txt" para o usuário logado.
 * @param username O nome do usuário.
 */
void listar_pedidos_usuario(const char *username) {
    // Aula 08: fopen() no modo "r" (read)
    FILE *arq = fopen("pedidos.txt", "r");
    if (arq == NULL) {
        printf("Nenhum pedido salvo encontrado.\n");
        return;
    }

    char linha[150];
    char userLido[50];
    int pedidoID = 1;
    int encontrou = 0;

    printf("\n--- Meus Pedidos Salvos ---\n");

    
    while (fgets(linha, sizeof(linha), arq)) {
        // Aula 06: sscanf para ler dados formatados de uma string
        if (sscanf(linha, "USER:%[^,],", userLido) == 1) {
            
            // Aula 06: strcmp para comparar strings
            if (strcmp(userLido, username) == 0) {
                encontrou = 1;
                printf("\n--- Pedido ID: %d ---\n", pedidoID);
                printf("%s", linha); // Imprime a linha USER:

                // Lê e imprime as linhas ITEM: até o delimitador
                while (fgets(linha, sizeof(linha), arq) && strncmp(linha, "---FIM---", 9) != 0) {
                    printf("  %s", linha);
                }
                pedidoID++;
            }
        }
    }

    if (!encontrou) {
        printf("Você não tem nenhum pedido salvo.\n");
    }
    fclose(arq);
}

/**
 * @brief Exclui um pedido salvo do "pedidos.txt" com base em um ID.
 * @param username O nome do usuário (para segurança).
 */
void excluir_pedido(const char *username) {
    // 1. Mostrar os pedidos para o usuário saber o ID
    listar_pedidos_usuario(username);
    
    int idParaExcluir;
    printf("\nDigite o ID do Pedido que deseja excluir (0 para cancelar): ");
    scanf("%d", &idParaExcluir);
    getchar();

    if (idParaExcluir == 0) {
        printf("Exclusão cancelada.\n");
        return;
    }

    // 2. Abrir arquivos (original para ler, temp para escrever)
    FILE *original = fopen("pedidos.txt", "r");
    FILE *temporario = fopen("temp.txt", "w"); 

    if (original == NULL || temporario == NULL) {
        printf("Erro ao processar arquivos.\n");
        if (original) fclose(original);
        if (temporario) fclose(temporario);
        return;
    }

    char linha[150];
    char userLido[50];
    int pedidoID = 1;
    int excluido = 0;
    
    // 3. Copiar dados, pulando o bloco a ser excluído
    while (fgets(linha, sizeof(linha), original)) { // <-- CORRIGIDO
        int manter = 1; 

        if (sscanf(linha, "USER:%[^,],", userLido) == 1) {
            if (strcmp(userLido, username) == 0) {
                if (pedidoID == idParaExcluir) {
                    manter = 0;
                    excluido = 1;
                }
                pedidoID++;
            }
        }
        
        if (manter) {
            fprintf(temporario, "%s", linha);
        } else {
            // Pula as linhas de ITEM até o fim do bloco
            while (fgets(linha, sizeof(linha), original) && strncmp(linha, "---FIM---", 9) != 0) { // <-- CORRIGIDO
                // Não faz nada, apenas consome as linhas
            }
        }
    }

    // 4. Fechar e substituir os arquivos
    fclose(original);
    fclose(temporario);

    if (excluido) {
        // Aula 08: remove() apaga um arquivo
        remove("pedidos.txt"); 
        // rename() é da stdio.h, muda o nome de "temp.txt" para "pedidos.txt"
        rename("temp.txt", "pedidos.txt"); 
        printf("Pedido ID %d excluído com sucesso.\n", idParaExcluir);
    } else {
        remove("temp.txt");
        printf("Pedido ID %d não encontrado para este usuário.\n", idParaExcluir);
    }
}