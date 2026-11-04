#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

#include "player.h"
#include "npc.h"
#include "mediator.h"

#include "inputmanager.h"
#include "aimanager.h"

#include "command.h"

#include "collision.h"

// Define GameData struct
typedef struct
{
    Player *player;
    NPC *npc;
    Mediator *playerMediator;
    Mediator *npcMediator;
} GameData;

// Function prototypes
void InitGame(GameData *data);
void UpdateGame(GameData *data, float deltaTime);
void DrawGame(const GameData *data);
void CloseGame(GameData *data);

#endif // GAME_H