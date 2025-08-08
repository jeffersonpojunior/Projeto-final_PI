#ifndef FASE2_H
#define FASE2_H

#include "raylib.h"
#include <stdlib.h>

// Função que inicia a 2º fase
void InitFase2(void);

// Retorna um status: 0 (rodando), 1 (fase vencida), 2 (fase perdida)
int UpdateFase2(void); // Função que atualiza a lógica de um frame da Fase 2

// Desenha um frame da Fase 2 na tela
void DrawFase2(void);

// Função que finaliza a 2º fase
void UnloadFase2(void);

#endif /* FASE2_h */