#include "menu.hpp"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const float BTN_LARGURA = 300.0f;
const float BTN_ALTURA  = 50.0f;
const float BTN_X       = LARGURA_TELA / 2.0f - BTN_LARGURA / 2.0f;
const float BTN_Y       = ALTURA_TELA  / 2.0f - BTN_ALTURA  / 2.0f;
const int   FONT_SIZE   = 20;

static const char* SERVIDOR_IP    = "56.126.130.33";
static const int   SERVIDOR_PORTA = 7777;

static int         sock        = -1;
static sockaddr_in addr_servidor;
static uint8_t     num_jogador = 0;

int          get_sock()           { return sock; }
sockaddr_in* get_addr_servidor()  { return &addr_servidor; }
uint8_t      get_num_jogador()    { return num_jogador; }


Tela processar_menu(Vector2 mouse, char* senha_criada) {
    Rectangle btnCriar  = { BTN_X, BTN_Y,      BTN_LARGURA, BTN_ALTURA };
    Rectangle btnEntrar = { BTN_X, BTN_Y + 70, BTN_LARGURA, BTN_ALTURA };

    if (CheckCollisionPointRec(mouse, btnCriar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        senha_criada[0] = '\0';
        return TELA_CRIAR;
    }
    if (CheckCollisionPointRec(mouse, btnEntrar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return TELA_ENTRAR;

    return TELA_MENU;
}

Tela processar_criar(Vector2 mouse) {
    Rectangle btnConfirmar = { BTN_X, BTN_Y,      BTN_LARGURA, BTN_ALTURA };
    Rectangle btnVoltar    = { BTN_X, BTN_Y + 70, BTN_LARGURA, BTN_ALTURA };

    if (CheckCollisionPointRec(mouse, btnConfirmar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        sock = criar_socket_cliente(SERVIDOR_IP, SERVIDOR_PORTA, &addr_servidor);
        PacketMatchmaking pkt = { PACOTE_CRIAR_SALA, {} };
        enviar_para(sock, &pkt, sizeof(pkt), &addr_servidor);
        return TELA_AGUARDANDO;
    }
    if (CheckCollisionPointRec(mouse, btnVoltar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return TELA_MENU;

    return TELA_CRIAR;
}

Tela processar_aguardando(char* senha_criada) {
    uint8_t    buffer[64];
    sockaddr_in remetente;
    int bytes = receber_de(sock, buffer, sizeof(buffer), &remetente);

    if (bytes > 0) {
        uint8_t tipo = buffer[0];

        if (tipo == PACOTE_SALA_OK) {
            PacketSalaOK* pkt = (PacketSalaOK*)buffer;
            memcpy(senha_criada, pkt->codigo, sizeof(pkt->codigo));
        }
        if (tipo == PACOTE_JOGO_INICIO) {
            PacketJogoInicio* pkt = (PacketJogoInicio*)buffer;
            num_jogador = pkt->num_jogador;
            return TELA_JOGO;
        }
    }

    return TELA_AGUARDANDO;
}

Tela processar_entrar(Vector2 mouse, char* senha_entrada, int* len) {
    int c = GetCharPressed();
    while (c > 0) {
        if (c >= '0' && c <= '9' && *len < 4) {
            senha_entrada[*len] = (char)c;
            (*len)++;
            senha_entrada[*len] = '\0';
        }
        c = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && *len > 0) {
        (*len)--;
        senha_entrada[*len] = '\0';
    }

    Rectangle btnEntrar = { BTN_X, BTN_Y,      BTN_LARGURA, BTN_ALTURA };
    Rectangle btnVoltar = { BTN_X, BTN_Y + 70, BTN_LARGURA, BTN_ALTURA };

    if (CheckCollisionPointRec(mouse, btnEntrar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && *len == 4) {
        sock = criar_socket_cliente(SERVIDOR_IP, SERVIDOR_PORTA, &addr_servidor);
        PacketMatchmaking pkt = { PACOTE_ENTRAR_SALA, {} };
        memcpy(pkt.codigo, senha_entrada, sizeof(pkt.codigo));
        enviar_para(sock, &pkt, sizeof(pkt), &addr_servidor);
        return TELA_AGUARDANDO;
    }
    if (CheckCollisionPointRec(mouse, btnVoltar) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *len = 0;
        senha_entrada[0] = '\0';
        return TELA_MENU;
    }

    return TELA_ENTRAR;
}

// ── DESENHAR ──────────────────────────────────────────────────────────────────

void desenhar_menu(Vector2 mouse) {
    Rectangle btnCriar  = { BTN_X, BTN_Y,      BTN_LARGURA, BTN_ALTURA };
    Rectangle btnEntrar = { BTN_X, BTN_Y + 70, BTN_LARGURA, BTN_ALTURA };

    bool hoverCriar  = CheckCollisionPointRec(mouse, btnCriar);
    bool hoverEntrar = CheckCollisionPointRec(mouse, btnEntrar);

    ClearBackground(BLACK);

    const char* titulo    = "PONG NET";
    const char* subtitulo = "Engenharia de Redes - UnB - 2026";
    DrawText(titulo,
        LARGURA_TELA / 2 - MeasureText(titulo, 60) / 2,
        80, 60, WHITE);
    DrawText(subtitulo,
        LARGURA_TELA / 2 - MeasureText(subtitulo, 25) / 2,
        155, 25, GRAY);

    DrawRectangleRec(btnCriar,  hoverCriar  ? GRAY : LIGHTGRAY);
    DrawRectangleRec(btnEntrar, hoverEntrar ? GRAY : LIGHTGRAY);

    const char* txtCriar  = "Criar Sala";
    const char* txtEntrar = "Entrar em Sala";
    DrawText(txtCriar,
        btnCriar.x  + (btnCriar.width  - MeasureText(txtCriar,  FONT_SIZE)) / 2,
        btnCriar.y  + (btnCriar.height - FONT_SIZE) / 2, FONT_SIZE, DARKGRAY);
    DrawText(txtEntrar,
        btnEntrar.x + (btnEntrar.width - MeasureText(txtEntrar, FONT_SIZE)) / 2,
        btnEntrar.y + (btnEntrar.height - FONT_SIZE) / 2, FONT_SIZE, DARKGRAY);
}

void desenhar_criar(Vector2 mouse, const char* senha) {
    Rectangle btnConfirmar = { BTN_X, BTN_Y,      BTN_LARGURA, BTN_ALTURA };
    Rectangle btnVoltar    = { BTN_X, BTN_Y + 70, BTN_LARGURA, BTN_ALTURA };

    bool hoverConfirmar = CheckCollisionPointRec(mouse, btnConfirmar);
    bool hoverVoltar    = CheckCollisionPointRec(mouse, btnVoltar);

    ClearBackground(BLACK);
    DrawText("Criar Sala",
        LARGURA_TELA / 2 - MeasureText("Criar Sala", 30) / 2, 80, 30, WHITE);
    DrawText("Código da sala:",
        LARGURA_TELA / 2 - MeasureText("Código da sala:", FONT_SIZE) / 2,
        BTN_Y - 80, FONT_SIZE, LIGHTGRAY);

    if (senha[0] != '\0')
        DrawText(senha,
            LARGURA_TELA / 2 - MeasureText(senha, 50) / 2,
            BTN_Y - 50, 50, YELLOW);
    else
        DrawText("...",
            LARGURA_TELA / 2 - MeasureText("...", 50) / 2,
            BTN_Y - 50, 50, DARKGRAY);

    DrawRectangleRec(btnConfirmar, hoverConfirmar ? GRAY : LIGHTGRAY);
    DrawText("Criar",
        btnConfirmar.x + (btnConfirmar.width  - MeasureText("Criar",  FONT_SIZE)) / 2,
        btnConfirmar.y + (btnConfirmar.height - FONT_SIZE) / 2, FONT_SIZE, DARKGRAY);

    DrawRectangleRec(btnVoltar, hoverVoltar ? GRAY : LIGHTGRAY);
    DrawText("Voltar",
        btnVoltar.x + (btnVoltar.width  - MeasureText("Voltar", FONT_SIZE)) / 2,
        btnVoltar.y + (btnVoltar.height - FONT_SIZE) / 2, FONT_SIZE, DARKGRAY);
}

void desenhar_aguardando(const char* senha) {
    ClearBackground(BLACK);
    DrawText("Código da sala:",
        LARGURA_TELA / 2 - MeasureText("Código da sala:", FONT_SIZE) / 2,
        ALTURA_TELA / 2 - 100, FONT_SIZE, LIGHTGRAY);
    DrawText(senha,
        LARGURA_TELA / 2 - MeasureText(senha, 50) / 2,
        ALTURA_TELA / 2 - 70, 50, YELLOW);
    DrawText("Esperando jogador...",
        LARGURA_TELA / 2 - MeasureText("Esperando jogador...", FONT_SIZE) / 2,
        ALTURA_TELA / 2 + 10, FONT_SIZE, WHITE);
}

void desenhar_entrar(Vector2 mouse, const char* senha_entrada) {
    Rectangle caixaInput = { BTN_X, BTN_Y - 80, BTN_LARGURA, BTN_ALTURA };
    Rectangle btnEntrar  = { BTN_X, BTN_Y,      BTN_LARGURA, BTN_ALTURA };
    Rectangle btnVoltar  = { BTN_X, BTN_Y + 70, BTN_LARGURA, BTN_ALTURA };

    bool hoverEntrar = CheckCollisionPointRec(mouse, btnEntrar);
    bool hoverVoltar = CheckCollisionPointRec(mouse, btnVoltar);

    ClearBackground(BLACK);
    DrawText("Entrar em Sala",
        LARGURA_TELA / 2 - MeasureText("Entrar em Sala", 30) / 2, 80, 30, WHITE);
    DrawText("Digite o código:",
        LARGURA_TELA / 2 - MeasureText("Digite o código:", FONT_SIZE) / 2,
        BTN_Y - 120, FONT_SIZE, LIGHTGRAY);

    DrawRectangleRec(caixaInput, DARKGRAY);
    DrawRectangleLinesEx(caixaInput, 2, LIGHTGRAY);
    DrawText(senha_entrada,
        caixaInput.x + (caixaInput.width - MeasureText(senha_entrada, 30)) / 2,
        caixaInput.y + (caixaInput.height - 30) / 2, 30, WHITE);

    DrawRectangleRec(btnEntrar, hoverEntrar ? GRAY : LIGHTGRAY);
    DrawText("Entrar",
        btnEntrar.x + (btnEntrar.width  - MeasureText("Entrar", FONT_SIZE)) / 2,
        btnEntrar.y + (btnEntrar.height - FONT_SIZE) / 2, FONT_SIZE, DARKGRAY);

    DrawRectangleRec(btnVoltar, hoverVoltar ? GRAY : LIGHTGRAY);
    DrawText("Voltar",
        btnVoltar.x + (btnVoltar.width  - MeasureText("Voltar", FONT_SIZE)) / 2,
        btnVoltar.y + (btnVoltar.height - FONT_SIZE) / 2, FONT_SIZE, DARKGRAY);
}

void desenhar_jogo(const EstadoJogo* estado) {
    Rectangle bastao1 = { BASTAO1_X, estado->bastao1_y, LARGURA_BASTAO, ALTURA_BASTAO };
    Rectangle bastao2 = { BASTAO2_X, estado->bastao2_y, LARGURA_BASTAO, ALTURA_BASTAO };

    ClearBackground(BLACK);

    for (int i = 10; i <= ALTURA_TELA - 10; i += 40)
        DrawLine(LARGURA_TELA / 2, i, LARGURA_TELA / 2, i + 20, WHITE);

    DrawRectangleRec(bastao1, WHITE);
    DrawRectangleRec(bastao2, WHITE);
    DrawCircle(estado->bola.x, estado->bola.y, RAIO_BOLA, WHITE);

    DrawText(TextFormat("%d", estado->placar1), LARGURA_TELA / 2 - 60, 10, 40, WHITE);
    DrawText(TextFormat("%d", estado->placar2), LARGURA_TELA / 2 + 40, 10, 40, WHITE);
}
