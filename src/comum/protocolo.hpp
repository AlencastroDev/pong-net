#pragma once
#include <stdint.h>

enum TipoPacote : uint8_t {
    
    PACOTE_CRIAR_SALA  = 1,
    PACOTE_ENTRAR_SALA = 2,
    PACOTE_SALA_OK     = 3,  // servidor → cliente: "sala criada, seu código é X"
    PACOTE_JOGO_INICIO = 4,  // servidor → cliente: "você é jogador N, começa agora"
    PACOTE_INPUT       = 5,
    PACOTE_ESTADO      = 6,
};

// Cliente → Servidor
struct PacketInput {
    uint8_t tipo;         // sempre PACOTE_INPUT
    uint8_t num_jogador;  // 1 ou 2
    uint8_t cima;
    uint8_t baixo;
};

// Servidor → Clientes
struct PacketEstado {
    uint8_t tipo;       // sempre PACOTE_ESTADO
    float   bola_x;
    float   bola_y;
    float   bastao1_y;
    float   bastao2_y;
    int32_t placar1;
    int32_t placar2;
};

// Cliente → Servidor: criar ou entrar em sala
struct PacketMatchmaking {
    uint8_t tipo;      // PACOTE_CRIAR_SALA ou PACOTE_ENTRAR_SALA
    char    codigo[5]; // vazio ao criar, preenchido ao entrar
};

// Servidor → Cliente: sala criada, aqui está o código
struct PacketSalaOK {
    uint8_t tipo;      // PACOTE_SALA_OK
    char    codigo[5];
};

// Servidor → Cliente: jogo vai começar
struct PacketJogoInicio {
    uint8_t tipo;        // PACOTE_JOGO_INICIO
    uint8_t num_jogador; // 1 ou 2
};
