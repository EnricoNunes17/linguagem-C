#include "feifood.h" 


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
        getchar(); 

        switch(opcao) {
            case 1: criar_novo_pedido(username); break;
            case 2: listar_pedidos_usuario(username); break;
            case 3: excluir_pedido(username); break;
            case 0: return; 
            default: printf("Opcao invalida.\n");
        }
    } while(1);
}


void criar_novo_pedido(const char *username) {
    
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


void adicionar_item_pedido(Pedido *p) {
    if (p->num_itens >= 20) {
        printf("Carrinho cheio!\n");
        return;
    }

    char nomeAlimento[50];
    printf("Digite o nome exato do alimento para adicionar: ");
    fgets(nomeAlimento, sizeof(nomeAlimento), stdin);
    nomeAlimento[strcspn(nomeAlimento, "\n")] = 0;

    
    Alimento al = buscar_alimento_por_nome(nomeAlimento);

    if (al.preco == 0) { 
        printf("Alimento '%s' não encontrado!\n", nomeAlimento);
    } else {
        
        p->itens[p->num_itens] = al; 
        p->num_itens++;
        p->total_preco += al.preco; 
        printf("%s (R$ %.2f) adicionado ao carrinho!\n", al.nome, al.preco);
    }
}


void remover_item_pedido(Pedido *p) {
    if (p->num_itens == 0) {
        printf("Carrinho já está vazio!\n");
        return;
    }
    
    p->num_itens--; 
    Alimento al = p->itens[p->num_itens]; 
    p->total_preco -= al.preco; 
    printf("Item %s (R$ %.2f) removido.\n", al.nome, al.preco);
}


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


void salvar_pedido(Pedido *p) {
    if (p->num_itens == 0) {
        printf("Não é possível salvar um pedido vazio.\n");
        return;
    }
    
    
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
    fprintf(arq, "---FIM---\n"); 
    
    fclose(arq); 
    printf("Pedido salvo com sucesso!\n");
} 
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
        
        if (sscanf(linha, "USER:%[^,],", userLido) == 1) {
            
            
            if (strcmp(userLido, username) == 0) {
                encontrou = 1;
                printf("\n--- Pedido ID: %d ---\n", pedidoID);
                printf("%s", linha); 
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


void excluir_pedido(const char *username) {
    
    listar_pedidos_usuario(username);
    
    int idParaExcluir;
    printf("\nDigite o ID do Pedido que deseja excluir (0 para cancelar): ");
    scanf("%d", &idParaExcluir);
    getchar();

    if (idParaExcluir == 0) {
        printf("Exclusão cancelada.\n");
        return;
    }

    
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
    
    
    while (fgets(linha, sizeof(linha), original)) { 
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
            
            while (fgets(linha, sizeof(linha), original) && strncmp(linha, "---FIM---", 9) != 0) { // <-- CORRIGIDO
                
            }
        }
    }

    
    fclose(original);
    fclose(temporario);

    if (excluido) {
        
        remove("pedidos.txt"); 
        
        rename("temp.txt", "pedidos.txt"); 
        printf("Pedido ID %d excluído com sucesso.\n", idParaExcluir);
    } else {
        remove("temp.txt");
        printf("Pedido ID %d não encontrado para este usuário.\n", idParaExcluir);
    }
}