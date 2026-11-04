#include <stdbool.h>
#include <stdlib.h>

#include "mediator.h"
#include "gameobject.h"
#include "hud.h"
#include "command.h"

// For a good explanation on Mediator Pattern see
// https://www.geeksforgeeks.org/mediator-design-pattern/
// https://refactoring.guru/design-patterns/mediator

// Creates and initializes a new mediator instance
Mediator *CreateMediator(GameObject *object, Hud *hud)
{
    Mediator *mediator = (Mediator *)malloc(sizeof(Mediator));
    mediator->object = object;
    mediator->hud = hud;
    return mediator;
}

// Function to handle command and what needs to be done with it. The mediator is the only place that knows about both the game object and the HUD.
void MediatorHandleCommand(Mediator *mediator, Command command, float deltaTime)
{
    // One command, two collaborators. Neither knows about the other:
    // The mediator is the only place that knows about both.

    // 1. Game object acts on the command (via the Command System, which sends FSM events)
    ExecuteCommand(command, mediator->object, deltaTime);

    // 2. HUD shows the command, and the name of the state the object is now in
    if (mediator->hud)
    {
        GameObject *object = mediator->object;
        UpdateHud(mediator->hud, command, object->stateConfigs[object->currentState].name);
    }
}
