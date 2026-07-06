#include "../src/comum/logica.hpp"
#include "../src/comum/protocolo.hpp"
#include "menu.hpp"
#include <raylib.h>
#include <stdlib.h>
#include <time.h>

int main() {
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Pong Net");
    SetTargetFPS(60);

    EstadoJogo estado;
    inicializar_estado(&estado);
    Tela tela_atual = TELA_MENU;

    InputJogador input = {};
    char senha_criada[5]  = {};
    char senha_entrada[5] = {};
    int  len_entrada = 0;
    srand(time(NULL));

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (tela_atual == TELA_JOGO) {
            input.cima1  = IsKeyDown(KEY_W);
            input.baixo1 = IsKeyDown(KEY_S);
            input.cima2  = IsKeyDown(KEY_UP);
            input.baixo2 = IsKeyDown(KEY_DOWN);

            // envia input pro servidor
            PacketInput pkt = { PACOTE_INPUT, get_num_jogador(),
                                (uint8_t)(get_num_jogador() == 1 ? input.cima1 : input.cima2),
                                (uint8_t)(get_num_jogador() == 1 ? input.baixo1 : input.baixo2) };
            enviar_para(get_sock(), &pkt, sizeof(pkt), get_addr_servidor());

            // recebe estado do servidor
            PacketEstado est;
            sockaddr_in rem;
            if (receber_de(get_sock(), &est, sizeof(est), &rem) > 0 && est.tipo == PACOTE_ESTADO) {
                estado.bola.x    = est.bola_x;
                estado.bola.y    = est.bola_y;
                estado.bastao1_y = est.bastao1_y;
                estado.bastao2_y = est.bastao2_y;
                estado.placar1   = est.placar1;
                estado.placar2   = est.placar2;
            }
        }

        switch (tela_atual) {
            case TELA_MENU:       tela_atual = processar_menu(mouse, senha_criada);                  break;
            case TELA_CRIAR:      tela_atual = processar_criar(mouse);                               break;
            case TELA_AGUARDANDO: tela_atual = processar_aguardando(senha_criada);                   break;
            case TELA_ENTRAR:     tela_atual = processar_entrar(mouse, senha_entrada, &len_entrada); break;
            case TELA_JOGO:       break;
        }

        BeginDrawing();
        switch (tela_atual) {
            case TELA_MENU:       desenhar_menu(mouse);                   break;
            case TELA_CRIAR:      desenhar_criar(mouse, senha_criada);    break;
            case TELA_AGUARDANDO: desenhar_aguardando(senha_criada);      break;
            case TELA_ENTRAR:     desenhar_entrar(mouse, senha_entrada);  break;
            case TELA_JOGO:       desenhar_jogo(&estado);                 break;
        }
        EndDrawing();
    }

    CloseWindow();
}
