#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 


typedef struct {
    char nome[50];
    char categoria[50];
    int calorias;
    double preco;
} Alimento;

typedef struct {
    char username[50];
    Alimento itens[20]; 
    int num_itens;
    double total_preco;
} Pedido;


void menu_principal();
void cadastrar_usuario();
void login_usuario();
void menu_usuario(const char *username);


void buscar_alimento();
void listar_info_alimento();
Alimento buscar_alimento_por_nome(const char *nome);
void str_tolower(char *str); 


void menu_gerenciar_pedidos(const char *username); 
void criar_novo_pedido(const char *username); 
void adicionar_item_pedido(Pedido *p);
void remover_item_pedido(Pedido *p);
void ver_carrinho(Pedido *p); 
void salvar_pedido(Pedido *p);
void listar_pedidos_usuario(const char *username); 
void excluir_pedido(const char *username); 


void avaliar_pedido(const char *username);