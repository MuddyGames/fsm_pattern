#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Include the header for the base game object
#include "gameobject.h"

// Define the Player structure that extends GameObject with additional properties like stamina and mana
typedef struct
{
    GameObject base; // The base game object (inherits from GameObject)
    float stamina;   // The player's stamina (could affect actions like moving, firing)
    float mana;      // The player's mana (could affect magic abilities or special moves)
} Player;

// Initialize a new Player with a given name (returns a pointer to the Player)
Player *InitPlayer(const char *name, float x, float y, int r);

// Initialize the finite state machine (FSM) for the Player (sets up the player's states)
void InitPlayerFSM(GameObject *object);

// Player-specific behaviors for different states

// Handle events in the idle state (when the player is not performing any action)
void PlayerIdleHandleEvent(GameObject *objectect, Event event, float deltaTime);

// State transition functions for idle state
void PlayerEnterIdle(GameObject *object, float deltaTime);  // Called when entering the idle state
void PlayerUpdateIdle(GameObject *object, float deltaTime); // Called to update the player's behavior while idle
void PlayerExitIdle(GameObject *object, float deltaTime);   // Called when exiting the idle state

// Handle events in the moving state (when the player is moving)
void PlayerMovingHandleEvent(GameObject *object, Event event, float deltaTime);

// State transition functions for moving state
void PlayerEnterMoving(GameObject *object, float deltaTime);  // Called when entering the moving state
void PlayerUpdateMoving(GameObject *object, float deltaTime); // Called to update the player's behavior while moving
void PlayerExitMoving(GameObject *object, float deltaTime);   // Called when exiting the moving state

// Handle events in the Firing state (when the player is Firing)
void PlayerFiringHandleEvent(GameObject *object, Event event, float deltaTime);

// State transition functions for Firing state
void PlayerEnterFiring(GameObject *object, float deltaTime);  // Called when entering the Firing state
void PlayerUpdateFiring(GameObject *object, float deltaTime); // Called to update the player's behavior while Firing
void PlayerExitFiring(GameObject *object, float deltaTime);   // Called when exiting the Firing state

// Handle events in the Jumping state (when the player is Jumping)
void PlayerJumpingHandleEvent(GameObject *object, Event event, float deltaTime);

// State transition functions for Jumping state
void PlayerEnterJumping(GameObject *object, float deltaTime);  // Called when entering the Jumping state
void PlayerUpdateJumping(GameObject *object, float deltaTime); // Called to update the player's behavior while Jumping
void PlayerExitJumping(GameObject *object, float deltaTime);   // Called when exiting the Jumping state

// Handle events in the die state (when the player dies)
void PlayerDieHandleEvent(GameObject *object, Event event, float deltaTime);

// State transition functions for die state
void PlayerEnterDie(GameObject *object, float deltaTime);  // Called when entering the die state
void PlayerUpdateDie(GameObject *object, float deltaTime); // Called to update the player's behavior while dead
void PlayerExitDie(GameObject *object, float deltaTime);   // Called when exiting the dead state

// Handle events in the respawn state (when the player respawns)
void PlayerRespawnHandleEvent(GameObject *object, Event event, float deltaTime);

// State transition functions for respawn state
void PlayerEnterRespawn(GameObject *object, float deltaTime);  // Called when entering the respawn state
void PlayerUpdateRespawn(GameObject *object, float deltaTime); // Called to update the player's behavior while respawning
void PlayerExitRespawn(GameObject *object, float deltaTime);   // Called when exiting the respawn

#endif