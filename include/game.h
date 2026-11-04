#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

#include "player.h"
#include "npc.h"
#include "hud.h"
#include "mediator.h"

#include "input_manager.h"
#include "ai_manager.h"

#include "command.h"

#include "collision.h"

// Define GameData struct
typedef struct
{
    Player *player;
    NPC *npc;
    Hud hud;
    Mediator *playerMediator;
    Mediator *npcMediator;
    Command activeCommand;
} GameData;

// Function prototypes
void InitGame(GameData *data);
void UpdateGame(GameData *data, float deltaTime);
void DrawGame(const GameData *data);
void CloseGame(GameData *data);

#endif // GAME_H