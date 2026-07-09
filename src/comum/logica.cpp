#include "logica.hpp"

void inicializar_estado(EstadoJogo* estado) {
    estado->bola.x              = LARGURA_TELA / 2.0f;
    estado->bola.y              = ALTURA_TELA / 2.0f;
    estado->bola.vel_x          = 8.f;
    estado->bola.vel_y          = 7.f;

    estado->bastao1_y           = ALTURA_TELA / 2.0f - ALTURA_BASTAO / 2.0f;
    estado->bastao2_y           = ALTURA_TELA / 2.0f - ALTURA_BASTAO / 2.0f;

    estado->placar1             = 0;
    estado->placar2             = 0;

    estado->pausado             = false;
    estado->timer_pausa         = 3.0f;
}

// Atualizar Estado
void atualizar_estado(EstadoJogo* estado, InputJogador* input){
    movimento_bastao(estado, input);
    movimento_bola(estado);
    chechar_colisao_bastao(estado);
}

void movimento_bola(EstadoJogo* estado) {
    checar_colisao_y(estado) ? estado->bola.vel_y *= -1 : estado->bola.vel_y;
    estado->bola.x += estado->bola.vel_x;
    estado->bola.y += estado->bola.vel_y;
    checar_colisao_x(estado);
}

bool checar_colisao_y(EstadoJogo* estado){
    if(estado->bola.y + RAIO_BOLA + estado->bola.vel_y >= ALTURA_TELA) return true;
    if(estado->bola.y - RAIO_BOLA + estado->bola.vel_y <= 0) return true;

    return false;
} 

void checar_colisao_x(EstadoJogo* estado){
    if(estado->bola.x - RAIO_BOLA <= 0) {
        estado->placar2++;
        ponto_marcado(estado);
    }

    if (estado->bola.x + RAIO_BOLA >= LARGURA_TELA) {
        estado->placar1++;
        ponto_marcado(estado);
    }
}

bool colide_circulo_retangulo(float cx, float cy, float raio, Retangulo r) {
    float prox_x = cx < r.x ? r.x : (cx > r.x + r.largura ? r.x + r.largura : cx);
    float prox_y = cy < r.y ? r.y : (cy > r.y + r.altura  ? r.y + r.altura  : cy);
    float dx = cx - prox_x;
    float dy = cy - prox_y;
    return (dx*dx + dy*dy) <= (raio * raio);
}

void chechar_colisao_bastao(EstadoJogo* estado) {
    Retangulo b1 = { BASTAO1_X, estado->bastao1_y, LARGURA_BASTAO, ALTURA_BASTAO };
    Retangulo b2 = { BASTAO2_X, estado->bastao2_y, LARGURA_BASTAO, ALTURA_BASTAO };

    if (colide_circulo_retangulo(estado->bola.x, estado->bola.y, RAIO_BOLA, b1))
        estado->bola.vel_x *= -1;

    if (colide_circulo_retangulo(estado->bola.x, estado->bola.y, RAIO_BOLA, b2))
        estado->bola.vel_x *= -1;
}

void ponto_marcado(EstadoJogo* estado){
    estado->bola.x   = LARGURA_TELA / 2.0f;
    estado->bola.y   = ALTURA_TELA  / 2.0f;
}

void movimento_bastao(EstadoJogo* estado, InputJogador* input){
   
    if (input->cima1 && estado->bastao1_y > 0)
        estado->bastao1_y -= VEL_BASTAO;
    if (input->baixo1 && estado->bastao1_y + ALTURA_BASTAO < ALTURA_TELA)
        estado->bastao1_y += VEL_BASTAO;

    if (input->cima2 && estado->bastao2_y > 0)
        estado->bastao2_y -= VEL_BASTAO;
    if (input->baixo2 && estado->bastao2_y + ALTURA_BASTAO < ALTURA_TELA)
        estado->bastao2_y += VEL_BASTAO;
}