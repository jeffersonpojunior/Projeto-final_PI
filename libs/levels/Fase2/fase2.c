#include "fase2.h"

#define MAX_PLAYER_BULLETS 100

// Abstrações das entidades do jogo
typedef struct {
    Rectangle rect;
    Vector2 speed;
    int health;
    bool active;
} Entity;

typedef struct {
    Rectangle rect;
    Vector2 speed;
    bool shot;
} Bullet;



// Texturas (Sprites)
static Texture2D playerSprite;
static Texture2D bossSprite;
static Texture2D bulletSprite;

// Entidades do Jogo
static Entity player;
static Entity boss;
static Bullet playerBullets[MAX_PLAYER_BULLETS]; // MAX_PLAYES_BULLETS bullets que o personagem dispõe

// Estado do Jogo
static bool gameOver;
static bool gameWin;
static float bossAttackTimer;




void InitFase2(void)
{
    // Carregando as sprites:
    playerSprite = LoadTexture("assets/images/player_ship.png");
    bossSprite = LoadTexture("assets/images/boss.png");
    bulletSprite = LoadTexture("assets/images/bullet.png");
    
    // Inicializando o player
    player.rect = (Rectangle){ 100, GetScreenHeight() / 2.0f, playerSprite.width, playerSprite.height };
    player.speed = (Vector2){ 0, 5.0f }; // 5 é a velocidade horizontal
    player.health = 100;
    player.active = true;

    // Inicializando o boss
    boss.rect = (Rectangle){ GetScreenWidth() - bossSprite.width - 50, GetScreenHeight() / 2.0f - bossSprite.height / 2.0f, bossSprite.width, bossSprite.height };
    boss.speed = (Vector2){ 0, 2.0f };
    boss.health = 500;
    boss.active = true;

    // Inicializando todas as bullets do player
    for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
    {
        playerBullets[i].shot = false;
    }

    // Estados do jogo
    gameOver = false;
    gameWin = false;
    bossAttackTimer = 0.0f;
}

int UpdateFase2(void) // Atualiza os frames/estados da fase 2 mas não desenha nada
{
    // Se o jogo já acabou, a função não faz nada
    if (gameOver) {
        return 2; // derrota
    }
    if (gameWin) {
        return 1;  // vitória
    }

    // --- Lógica do Jogador ---
    // Movimento (runner vertical)
    if (IsKeyDown(KEY_UP) && player.rect.y > 0) {
        player.rect.y -= player.speed.y;
    }
    if (IsKeyDown(KEY_DOWN) && player.rect.y < GetScreenHeight() - player.rect.height) {
        player.rect.y += player.speed.y;
    }

    // Configurando os tiros
    if (IsKeyPressed(KEY_SPACE))
    {
        for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
        {
            if (!playerBullets[i].shot)
            {
                playerBullets[i].shot = true;
                playerBullets[i].rect = (Rectangle){ player.rect.x + player.rect.width, player.rect.y + player.rect.height / 2, bulletSprite.width, bulletSprite.height };
                playerBullets[i].speed = (Vector2){10.0f, 0};
                // PlaySound(shootSound); // Adicionar som de tiro
                break; // Sai do loop para atirar apenas uma bala por vez
            }
        }
    }

    // Lógica do tiros:
    for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
    {
        if (playerBullets[i].shot)
        {
            // Movendo o tiro
            playerBullets[i].rect.x += playerBullets[i].speed.x;

            // Desativar se sair da tela
            if (playerBullets[i].rect.x > GetScreenWidth())
            {
                playerBullets[i].shot = false;
            }

            // Checando se houve colisão do tiro com o boss
            if (CheckCollisionRecs(playerBullets[i].rect, boss.rect))
            {
                playerBullets[i].shot = false;
                boss.health -= 10;
                // PlaySound(bossHitSound); // Adicionar som de dano
                if (boss.health <= 0)
                {
                    gameWin = true;
                }
            }
        }
    }
    
    // Movimento vertical do boss
    boss.rect.y += boss.speed.y;
    if (boss.rect.y < 0 || boss.rect.y > GetScreenHeight() - boss.rect.height)
    {
        boss.speed.y *= -1; // Inverte a direção ao bater na borda
    }

    // (Aqui você adicionaria a lógica de ataque do chefe, colisões com o jogador, etc.)
    // Ex: if (CheckCollisionRecs(player.rect, boss.rect)) gameOver = true;

    return 0; // Jogo ainda está rodando
}

void DrawFase2(void)
{
    BeginDrawing();
        ClearBackground(DARKBLUE); // Fundo da fase

        // Desenhando as entidades:
        DrawTextureRec(playerSprite, (Rectangle){0, 0, player.rect.width, player.rect.height}, (Vector2){player.rect.x, player.rect.y}, WHITE);
        DrawTextureRec(bossSprite, (Rectangle){0, 0, boss.rect.width, boss.rect.height}, (Vector2){boss.rect.x, boss.rect.y}, WHITE);

        // Desenhando os tiros
        for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
        {
            if (playerBullets[i].shot)
            {
                DrawTextureRec(bulletSprite, (Rectangle){0, 0, playerBullets[i].rect.width, playerBullets[i].rect.height}, (Vector2){playerBullets[i].rect.x, playerBullets[i].rect.y}, ORANGE);
            }
        }
        
        // Desenhar UI (Interface)
        DrawText(TextFormat("Boss_HP: %d", boss.health), GetScreenWidth() - 250, 10, 20, RED);
        
        if (gameOver) DrawText("GAME OVER", GetScreenWidth()/2 - MeasureText("GAME OVER", 40)/2, GetScreenHeight()/2 - 20, 40, RED);
        if (gameWin) DrawText("VOCÊ VENCEU!", GetScreenWidth()/2 - MeasureText("VOCÊ VENCEU!", 40)/2, GetScreenHeight()/2 - 20, 40, GREEN);

    EndDrawing();
}

void UnloadFase2(void)
{
    UnloadTexture(playerSprite);
    UnloadTexture(bossSprite);
    UnloadTexture(bulletSprite);
    // UnloadSound(...);
}