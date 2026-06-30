#pragma once
#include <stdint.h>

enum TipoPacote : uint8_t {
    PACOTE_INPUT  = 1,
    PACOTE_ESTADO = 2,
    // Fase 4 — matchmaking:
    // PACOTE_CRIAR_SALA    = 3,
    // PACOTE_ENTRAR_SALA   = 4,
    // PACOTE_SALA_CRIADA   = 5,
    // PACOTE_JOGO_INICIADO = 6,
};

// Cliente → Servidor: enviado todo frame enquanto o jogo estiver rodando
struct PacketInput {
    uint8_t tipo;         // sempre PACOTE_INPUT
    uint8_t num_jogador;  // 1 ou 2
    uint8_t cima;
    uint8_t baixo;
};

// Servidor → Clientes: estado autoritativo após processar os inputs do frame
struct PacketEstado {
    uint8_t tipo;       // sempre PACOTE_ESTADO
    float   bola_x;
    float   bola_y;
    float   bastao1_y;
    float   bastao2_y;
    int32_t placar1;
    int32_t placar2;
};
