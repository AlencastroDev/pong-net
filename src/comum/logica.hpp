#pragma once

const int LARGURA_TELA      = 1280;
const int ALTURA_TELA       = 800;
const int ALTURA_BASTAO     = 125;
const int LARGURA_BASTAO    = 8;
const int BASTAO1_X         = 20;
const int BASTAO2_X         = LARGURA_TELA - LARGURA_BASTAO - 20;
const int RAIO_BOLA         = 6;
const int VEL_BASTAO        = 12;

struct Retangulo{
    float x, y, largura, altura;
};

enum Tela{
    TELA_MENU,
    TELA_CRIAR,
    TELA_ENTRAR,
    TELA_AGUARDANDO,
    TELA_JOGO
};

struct Bola{
    float x, y, vel_x, vel_y;
};

struct InputJogador{
    bool cima1, baixo1;
    bool cima2, baixo2;
};

struct EstadoJogo{
    Bola bola;
    float bastao1_y;
    float bastao2_y;
    int placar1;
    int placar2;
    bool pausado;
    float timer_pausa;
};

void inicializar_estado(EstadoJogo* estado);
void atualizar_estado(EstadoJogo* estado, InputJogador* input);
void movimento_bola(EstadoJogo* estado);
bool checar_colisao_y(EstadoJogo* estado);
void checar_colisao_x(EstadoJogo* estado);
void ponto_marcado(EstadoJogo* estado);
void movimento_bastao(EstadoJogo* estado, InputJogador* input);
bool colide_circulo_retangulo(float cx, float cy, float raio, Retangulo r);
void chechar_colisao_bastao(EstadoJogo* estado);