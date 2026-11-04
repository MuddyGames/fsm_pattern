#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <stdbool.h>

#include "command.h"
#include "gameobject.h"

// Define the Mediator structure
typedef struct Mediator
{
    GameObject *object;
} Mediator;

// Function to create a mediator instance
Mediator *CreateMediator(GameObject *object);

// Execute Command
//void MediatorExecuteCommand(Command command, Mediator *mediator);
void MediatorHandleCommand(Mediator *mediator, Command command, float deltaTime);

// Mediator Cleanup
void DeleteMediator(Mediator *mediator);

#endif