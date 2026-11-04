#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <raylib.h>
#include <raymath.h>

#include "constants.h"

#include "events.h"
#include "fsm.h"

#include "cute_c2.h"

// Base structure for a game object
typedef struct GameObject
{
    const char *name; // The name of the game object (e.g., "Player", "Enemy")

    float x; // X Position
    float y; // Y Position
    int r;   // Radius

    c2Circle collider;   // Circle collider used for collision detection (also for drawing, if a position varible is used they must be sync'd in update)
    c2Manifold manifold; // Manifold for finding incident normals

    Vector2 inputAxis; // Input Axis (WASD / Arrow Keys / D-Pad and Tumbstick)
    Vector2 direction; // Direction of Movement

    Color color; // GameObject Color

    float timer; // Used during updates

    State previousState; // The state the game object was previously in
    State currentState;  // The current state of the game object

    StateConfig *stateConfigs; // Pointer to the array of state configurations for this game object

    bool isColliding; // Is Colliding

    int health; // The health of the game object

} GameObject;

// Initialize a new game object with the given name and default values
void InitGameObject(GameObject *obj, const char *name);

// Keep on screen
void ClampGameObjectOnScreen(GameObject *obj);

// Delete a game object and free associated memory/resources
void DeleteGameObject(GameObject *obj);

#endif