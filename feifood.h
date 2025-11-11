// feifood.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para tolower()

// --- Estruturas (Aula 09 - Structs) [cite: 237-238] ---
typedef struct {
    char nome[50];
    char categoria[50];
    int calorias;
    double preco;
} Alimento;

typedef struct {
    char username[50];
    Alimento itens[20]; // Array de structs (Aula 05) [cite: 24, 26]
    int num_itens;
    double total_preco;
} Pedido;

// --- Funções de Usuario (usuarios.c) ---
void menu_principal();
void cadastrar_usuario();
void login_usuario();
void menu_usuario(const char *username);

// --- Funções de Alimento (alimentos.c) ---
void buscar_alimento();
void listar_info_alimento();
Alimento buscar_alimento_por_nome(const char *nome);
void str_tolower(char *str); // Você já tinha essa, mantendo

// --- Funções de Pedido (pedidos.c) ---
void menu_gerenciar_pedidos(const char *username); // NOVO MENU
void criar_novo_pedido(const char *username); // Antiga 'cadastrar_pedido'
void adicionar_item_pedido(Pedido *p);
void remover_item_pedido(Pedido *p);
void ver_carrinho(Pedido *p); // NOVA
void salvar_pedido(Pedido *p);
void listar_pedidos_usuario(const char *username); // NOVA
void excluir_pedido(const char *username); // NOVA


void avaliar_pedido(const char *username);