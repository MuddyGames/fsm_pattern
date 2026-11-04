#ifndef NPC_H
#define NPC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Include the header for the base game object
#include "gameobject.h"

// Define the NPC structure that extends GameObject with an additional aggression property
typedef struct
{
    GameObject base; // The base game object (inherits from GameObject)
    int aggression;  // The aggression level of the NPC (could affect behavior)
} NPC;

// Initialize a new NPC with a given name (returns a pointer to the NPC)
NPC *InitNPC(const char *name, float x, float y, int r);

// Initialize NPC-specific states for the given GameObject
void InitNPCFSM(GameObject *object);

// NPC-specific behaviors for different states

// Handle events in the idle state
void NPCIdleHandleEvent(GameObject *object, Event event, float deltaTime);

// State transition functions for idle state
void NPCEnterIdle(GameObject *object, float deltaTime);
void NPCUpdateIdle(GameObject *object, float deltaTime);
void NPCExitIdle(GameObject *object, float deltaTime);

// Handle events in the firing state
void NPCFiringHandleEvent(GameObject *object, Event event, float deltaTime);

// State transition functions for firing state
void NPCEnterFiring(GameObject *object, float deltaTime);
void NPCUpdateFiring(GameObject *object, float deltaTime);
void NPCExitFiring(GameObject *object, float deltaTime);

// Handle events in the dead state
void NPCDeadHandleEvent(GameObject *object, Event event, float deltaTime);

// State transition functions for dead state
void NPCEnterDead(GameObject *object, float deltaTime);
void NPCUpdateDead(GameObject *object, float deltaTime);
void NPCExitDead(GameObject *object, float deltaTime);

#endif