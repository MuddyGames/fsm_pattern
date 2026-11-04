#include "gameobject.h"

// Initializes a GameObject with default values and assigns a name
void InitGameObject(GameObject *obj, const char *name)
{
    // Set the GameObject's name
    obj->name = name;

    // Initialize the previous and current states to IDLE
    obj->previousState = STATE_IDLE;
    obj->currentState = STATE_IDLE;

    // Set the initial health to 100
    obj->health = 100;
}

// Keep on screen
void ClampGameObjectOnScreen(GameObject *obj)
{
	if (obj->x < obj->r)
		obj->x = obj->r;
	if (obj->x > SCREEN_WIDTH - obj->r)
		obj->x = SCREEN_WIDTH - obj->r;
	if (obj->y < obj->r)
		obj->y = obj->r;
	if (obj->y > SCREEN_HEIGHT - obj->r)
		obj->y = SCREEN_HEIGHT - obj->r;
}

// Frees memory associated with a GameObject, including state configurations
void DeleteGameObject(GameObject *obj)
{
    // Check if state configurations exist for this GameObject
    if (obj->stateConfigs)
    {
        // Free each state's nextStates array
        for (int i = 0; i < STATE_COUNT; i++)
        {
            free(obj->stateConfigs[i].nextStates);
        }
        // Free the array of state configurations itself
        free(obj->stateConfigs);
    }
    // Free the GameObject memory
    free(obj);
}
