#pragma once
#include "../src/comum/logica.hpp"
#include "../src/comum/rede.hpp"
#include "../src/comum/protocolo.hpp"
#include <raylib.h>

Tela processar_menu(Vector2 mouse, char* senha_criada);
Tela processar_criar(Vector2 mouse);
Tela processar_aguardando(char* senha_criada);
Tela processar_entrar(Vector2 mouse, char* senha_entrada, int* len);

int          get_sock();
sockaddr_in* get_addr_servidor();
uint8_t      get_num_jogador();

void desenhar_menu(Vector2 mouse);
void desenhar_criar(Vector2 mouse, const char* senha);
void desenhar_aguardando(const char* senha);
void desenhar_entrar(Vector2 mouse, const char* senha_entrada);
void desenhar_jogo(const EstadoJogo* estado);
