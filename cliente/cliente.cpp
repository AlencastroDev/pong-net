#include "../src/comum/logica.hpp"
#include <raylib.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() {
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Pong Net");
    SetTargetFPS(60);

    EstadoJogo estado;
    inicializar_estado(&estado);
    Tela tela_atual = TELA_MENU;

    InputJogador input = {};
    char senha_criada[8] = {};
    char senha_entrada[8] = {};
    int len_entrada = 0;
    srand(time(NULL));

    while (!WindowShouldClose()) {

        if(tela_atual == TELA_JOGO){
            float dt = GetFrameTime();

            // --- INPUT ---
            // jogador 1: W / S
            // jogador 2: seta cima / seta baixo
            input.cima1 = IsKeyDown(KEY_W);
            input.baixo1 = IsKeyDown(KEY_S);

            input.cima2 = IsKeyDown(KEY_UP);
            input.baixo2 = IsKeyDown(KEY_DOWN);

            // --- LÓGICA ---
            atualizar_estado(&estado, &input);   
        }

       
        // converter Retangulo próprio → Rectangle do Raylib
        Rectangle bastao1 = { BASTAO1_X, estado.bastao1_y, LARGURA_BASTAO, ALTURA_BASTAO };
        Rectangle bastao2 = { BASTAO2_X, estado.bastao2_y, LARGURA_BASTAO, ALTURA_BASTAO };

        
        const float BTN_LARGURA = 300.0f;
        const float BTN_ALTURA  = 50.0f;
        const float BTN_X       = LARGURA_TELA / 2.0f - BTN_LARGURA / 2.0f;
        const float BTN_Y       = ALTURA_TELA  / 2.0f - BTN_ALTURA  / 2.0f;

        Rectangle btnCriar  = { BTN_X, BTN_Y,      BTN_LARGURA, BTN_ALTURA };
        Rectangle btnEntrar = { BTN_X, BTN_Y + 70, BTN_LARGURA, BTN_ALTURA };

        Vector2 mousePoint = GetMousePosition();
        Tela tela_frame = tela_atual;

        bool mouseOnCriar = CheckCollisionPointRec(mousePoint, btnCriar);
        bool mouseOnEntrar = CheckCollisionPointRec(mousePoint, btnEntrar);

        Rectangle btnConfirmar = { BTN_X, BTN_Y,      BTN_LARGURA, BTN_ALTURA };
        Rectangle btnVoltar    = { BTN_X, BTN_Y + 70, BTN_LARGURA, BTN_ALTURA };
        Rectangle caixaTexto   = { BTN_X, BTN_Y - 80, BTN_LARGURA, BTN_ALTURA };

        bool mouseOnConfirmar = CheckCollisionPointRec(mousePoint, btnConfirmar);
        bool mouseOnVoltar    = CheckCollisionPointRec(mousePoint, btnVoltar);
        bool mouseOnTexto     = CheckCollisionPointRec(mousePoint, caixaTexto);

        if (tela_frame == TELA_MENU) {
            if (mouseOnCriar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                sprintf(senha_criada, "%06d", rand() % 1000000);
                tela_atual = TELA_CRIAR;
            }
            if (mouseOnEntrar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                senha_entrada[0] = '\0';
                len_entrada = 0;
                tela_atual = TELA_ENTRAR;
            }
        }

        if (tela_frame == TELA_CRIAR) {
            if (mouseOnConfirmar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) tela_atual = TELA_AGUARDANDO;
            if (mouseOnVoltar    && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) tela_atual = TELA_MENU;
        }

        if (tela_frame == TELA_ENTRAR) {
            SetMouseCursor(mouseOnTexto ? MOUSE_CURSOR_IBEAM : MOUSE_CURSOR_DEFAULT);

            int c = GetCharPressed();
            while (c > 0) {
                if (c >= '0' && c <= '9' && len_entrada < 6) {
                    senha_entrada[len_entrada] = (char)c;
                    len_entrada++;
                    senha_entrada[len_entrada] = '\0';
                }
                c = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && len_entrada > 0) {
                len_entrada--;
                senha_entrada[len_entrada] = '\0';
            }

            if (mouseOnConfirmar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && len_entrada == 6)
                tela_atual = TELA_AGUARDANDO;
            if (mouseOnVoltar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                tela_atual = TELA_MENU;
        }

        const char* textoCriar = "Criar Sala";
        const char* textoEntrar = "Entrar em Sala";
        int fontSize = 20;

            
        // --- RENDER ---
        BeginDrawing();

        switch (tela_atual)
        {
            case TELA_MENU:
                ClearBackground(BLACK);
                DrawRectangleRec(btnCriar, !mouseOnCriar ? LIGHTGRAY : GRAY);
                DrawRectangleRec(btnEntrar, !mouseOnEntrar ? LIGHTGRAY : GRAY);
                DrawText(textoCriar, 
                    btnCriar.x + (btnCriar.width - MeasureText(textoCriar, fontSize)) / 2, 
                    btnCriar.y + (btnCriar.height - fontSize) /2, 
                    fontSize, DARKGRAY);
                DrawText(textoEntrar, 
                    btnEntrar.x + (btnEntrar.width - MeasureText(textoEntrar, fontSize)) / 2, 
                    btnEntrar.y + (btnEntrar.height - fontSize) /2, 
                    fontSize, DARKGRAY);
                break;
            
            case TELA_CRIAR:
                ClearBackground(BLACK);
                DrawText("Criar Sala",
                    LARGURA_TELA / 2 - MeasureText("Criar Sala", 30) / 2, 80, 30, WHITE);
                DrawText("Código da sala:",
                    LARGURA_TELA / 2 - MeasureText("Código da sala:", fontSize) / 2,
                    BTN_Y - 80, fontSize, LIGHTGRAY);
                DrawText(senha_criada,
                    LARGURA_TELA / 2 - MeasureText(senha_criada, 50) / 2,
                    BTN_Y - 50, 50, YELLOW);
                DrawRectangleRec(btnConfirmar, !mouseOnConfirmar ? LIGHTGRAY : GRAY);
                DrawText("Criar",
                    btnConfirmar.x + (btnConfirmar.width  - MeasureText("Criar",  fontSize)) / 2,
                    btnConfirmar.y + (btnConfirmar.height - fontSize) / 2, fontSize, DARKGRAY);
                DrawRectangleRec(btnVoltar, !mouseOnVoltar ? LIGHTGRAY : GRAY);
                DrawText("Voltar",
                    btnVoltar.x + (btnVoltar.width  - MeasureText("Voltar", fontSize)) / 2,
                    btnVoltar.y + (btnVoltar.height - fontSize) / 2, fontSize, DARKGRAY);
                break;

            case TELA_AGUARDANDO:
                ClearBackground(BLACK);
                DrawText("Código da sala:",
                    LARGURA_TELA / 2 - MeasureText("Código da sala:", fontSize) / 2,
                    ALTURA_TELA / 2 - 100, fontSize, LIGHTGRAY);
                DrawText(senha_criada,
                    LARGURA_TELA / 2 - MeasureText(senha_criada, 50) / 2,
                    ALTURA_TELA / 2 - 70, 50, YELLOW);
                DrawText("Esperando jogador...",
                    LARGURA_TELA / 2 - MeasureText("Esperando jogador...", fontSize) / 2,
                    ALTURA_TELA / 2 + 10, fontSize, WHITE);
                break;

            case TELA_ENTRAR:
                ClearBackground(BLACK);
                DrawText("Entrar em Sala",
                    LARGURA_TELA / 2 - MeasureText("Entrar em Sala", 30) / 2, 80, 30, WHITE);
                DrawText("Digite o codigo da sala:",
                    LARGURA_TELA / 2 - MeasureText("Digite o codigo da sala:", fontSize) / 2,
                    BTN_Y - 120, fontSize, LIGHTGRAY);
                DrawRectangleRec(caixaTexto, mouseOnTexto ? GRAY : LIGHTGRAY);
                DrawText(senha_entrada,
                    caixaTexto.x + 10,
                    caixaTexto.y + (caixaTexto.height - 30) / 2,
                    30, DARKGRAY);
                DrawRectangleRec(btnConfirmar, !mouseOnConfirmar ? LIGHTGRAY : GRAY);
                DrawText("Entrar",
                    btnConfirmar.x + (btnConfirmar.width  - MeasureText("Entrar", fontSize)) / 2,
                    btnConfirmar.y + (btnConfirmar.height - fontSize) / 2, fontSize, DARKGRAY);
                DrawRectangleRec(btnVoltar, !mouseOnVoltar ? LIGHTGRAY : GRAY);
                DrawText("Voltar",
                    btnVoltar.x + (btnVoltar.width  - MeasureText("Voltar", fontSize)) / 2,
                    btnVoltar.y + (btnVoltar.height - fontSize) / 2, fontSize, DARKGRAY);
                break;

            case TELA_JOGO:
                ClearBackground(BLACK);

                for (int i = 10; i <= ALTURA_TELA - 10; i += 40)
                    DrawLine(LARGURA_TELA / 2, i, LARGURA_TELA / 2, i + 20, WHITE);

                DrawRectangleRec(bastao1, WHITE);
                DrawRectangleRec(bastao2, WHITE);

                DrawCircle(estado.bola.x, estado.bola.y, RAIO_BOLA, WHITE);

                DrawText(TextFormat("%d", estado.placar1), LARGURA_TELA / 2 - 60, 10, 40, WHITE);
                DrawText(TextFormat("%d", estado.placar2), LARGURA_TELA / 2 + 40, 10, 40, WHITE);
                break;
            default:
                break;
            }
        EndDrawing();
    }

    CloseWindow();
}
