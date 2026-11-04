#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <stdbool.h>

#include "command.h"
#include "gameobject.h"
#include "hud.h"

// Define the Mediator structure
// Mediator knows about (game object and HUD).
// Game object and HUD don't know about each other.
// The NPC's mediator has no HUD (hud is NULL).
typedef struct Mediator
{
    GameObject *object;
    Hud *hud;
} Mediator;

// Function to create a mediator instance (hud can be NULL)
Mediator *CreateMediator(GameObject *object, Hud *hud);

//void MediatorExecuteCommand(Mediator *mediator, Command command, float deltaTime);
void MediatorHandleCommand(Mediator *mediator, Command command, float deltaTime);
// Mediator Cleanup
void DeleteMediator(Mediator *mediator);
#endif
