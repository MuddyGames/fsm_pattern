#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "mediator.h"
#include "command.h"

// For a good explanation on Mediator Pattern see
// https://www.geeksforgeeks.org/mediator-design-pattern/

// Creates and initializes a new mediator instance
Mediator *CreateMediator(GameObject *object)
{
    Mediator *mediator = (Mediator *)malloc(sizeof(Mediator));
    mediator->object = object;
    return mediator;
}

// Function to execute a command through the mediator
void MediatorHandleCommand(Mediator *mediator, Command command, float deltaTime)
{
    ExecuteCommand(mediator, command, deltaTime);
}

// Mediator cleanup
void DeleteMediator(Mediator *mediator)
{
    // Free Mediator
	if (mediator)
	{
		free(mediator);
		mediator = NULL;
	}
}
