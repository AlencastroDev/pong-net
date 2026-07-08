#include "../src/comum/logica.hpp"
#include "../src/comum/protocolo.hpp"
#include "../src/comum/rede.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

const int PORTA     = 7777;
const int MAX_SALAS = 10;

struct Sala {
    char         codigo[5];
    sockaddr_in  jogador1;
    sockaddr_in  jogador2;
    bool         j1_conectado;
    bool         j2_conectado;
    bool         em_jogo;
    EstadoJogo   estado;
    InputJogador ultimo_input;
};

static Sala salas[MAX_SALAS];
static int  num_salas = 0;

// ── HELPERS ───────────────────────────────────────────────────────────────────

bool mesmo_endereco(const sockaddr_in* a, const sockaddr_in* b) {
    return a->sin_addr.s_addr == b->sin_addr.s_addr &&
           a->sin_port         == b->sin_port;
}

Sala* buscar_por_codigo(const char* codigo) {
    for (int i = 0; i < num_salas; i++)
        if (memcmp(salas[i].codigo, codigo, 5) == 0)
            return &salas[i];
    return NULL;
}

Sala* buscar_por_jogador(const sockaddr_in* addr) {
    for (int i = 0; i < num_salas; i++) {
        if (!salas[i].em_jogo) continue;
        if (mesmo_endereco(&salas[i].jogador1, addr)) return &salas[i];
        if (mesmo_endereco(&salas[i].jogador2, addr)) return &salas[i];
    }
    return NULL;
}

void broadcast_estado(int sock, Sala* sala) {
    PacketEstado pkt;
    pkt.tipo      = PACOTE_ESTADO;
    pkt.bola_x    = sala->estado.bola.x;
    pkt.bola_y    = sala->estado.bola.y;
    pkt.bastao1_y = sala->estado.bastao1_y;
    pkt.bastao2_y = sala->estado.bastao2_y;
    pkt.placar1   = sala->estado.placar1;
    pkt.placar2   = sala->estado.placar2;
    enviar_para(sock, &pkt, sizeof(pkt), &sala->jogador1);
    enviar_para(sock, &pkt, sizeof(pkt), &sala->jogador2);
}

void gerar_codigo_Sala(char *buffer, size_t tamanho){
    // O '4' garante 4 dígitos, o '0' preenche com zeros à esquerda
    snprintf(buffer, tamanho, "%04d", rand() % 10000);
};

// ── HANDLERS DE PACOTE ────────────────────────────────────────────────────────

void handle_criar_sala(int sock, const sockaddr_in* remetente) {
    if (num_salas >= MAX_SALAS) return;

    Sala* sala = &salas[num_salas++];
    memset(sala, 0, sizeof(Sala));

    gerar_codigo_Sala(sala->codigo, sizeof(sala->codigo));

    sala->jogador1     = *remetente;
    sala->j1_conectado = true;
    inicializar_estado(&sala->estado);

    PacketSalaOK resp = { PACOTE_SALA_OK, {} };
    memcpy(resp.codigo, sala->codigo, sizeof(resp.codigo));
    enviar_para(sock, &resp, sizeof(resp), remetente);

    cout<<"Sala criada: "<<sala->codigo;
}

void handle_entrar_sala(int sock, const uint8_t* buffer, const sockaddr_in* remetente) {
    PacketMatchmaking* pkt  = (PacketMatchmaking*)buffer;
    Sala*              sala = buscar_por_codigo(pkt->codigo);

    if (sala == NULL) { cout<<"Sala: "<<pkt->codigo<<"nao encontrada"; return; }
    if (sala->j2_conectado) { cout<<"Sala: "<<pkt->codigo<<"esta cheia"; return; }

    sala->jogador2     = *remetente;
    sala->j2_conectado = true;
    sala->em_jogo      = true;

    PacketJogoInicio p1 = { PACOTE_JOGO_INICIO, 1 };
    PacketJogoInicio p2 = { PACOTE_JOGO_INICIO, 2 };
    enviar_para(sock, &p1, sizeof(p1), &sala->jogador1);
    enviar_para(sock, &p2, sizeof(p2), &sala->jogador2);

    cout<<"Sala: "<<pkt->codigo<<"iniciada";
}

void handle_input(const uint8_t* buffer, const sockaddr_in* remetente) {
    PacketInput* pkt  = (PacketInput*)buffer;
    Sala*        sala = buscar_por_jogador(remetente);
    if (sala == NULL) return;

    if (pkt->num_jogador == 1) {
        sala->ultimo_input.cima1  = pkt->cima;
        sala->ultimo_input.baixo1 = pkt->baixo;
    } else {
        sala->ultimo_input.cima2  = pkt->cima;
        sala->ultimo_input.baixo2 = pkt->baixo;
    }
}

int main() {
    srand(time(NULL));

    int sock = criar_socket_servidor(PORTA);
    cout<<"Servidor rodando na porta: "<<PORTA<<endl;

    uint8_t     buffer[256];
    sockaddr_in remetente;

    while (true) {
        int bytes;
        while ((bytes = receber_de(sock, buffer, sizeof(buffer), &remetente)) > 0) {
            uint8_t tipo = buffer[0];
            cout << "Pacote recebido, tipo=" << (int)tipo << endl;
            if      (tipo == PACOTE_CRIAR_SALA)   handle_criar_sala(sock, &remetente);
            else if (tipo == PACOTE_ENTRAR_SALA)  handle_entrar_sala(sock, buffer, &remetente);
            else if (tipo == PACOTE_INPUT)        handle_input(buffer, &remetente);
        }

        // avança e transmite o estado de cada sala em jogo
        for (int i = 0; i < num_salas; i++) {
            if (!salas[i].em_jogo) continue;
            atualizar_estado(&salas[i].estado, &salas[i].ultimo_input);
            broadcast_estado(sock, &salas[i]);
        }

        usleep(16666); // ~60 ticks/s
    }
}
