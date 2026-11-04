#include <stdbool.h>

#include "command.h"
#include "fsm.h"
#include "mediator.h"

// Function to execute a command
void ExecuteCommand(Mediator *mediator, Command command, float deltaTime)
{
	// Axis of Movement
	Vector2 axis = (Vector2){0.0f, 0.0f};

	if (IsCommandActive(command, MOVE_UP))
		axis = Vector2Add(axis, (Vector2){0.0f, -1.0f});

	if (IsCommandActive(command, MOVE_DOWN))
		axis = Vector2Add(axis, (Vector2){0.0f, 1.0f});

	if (IsCommandActive(command, MOVE_LEFT))
		axis = Vector2Add(axis, (Vector2){-1.0f, 0.0f});

	if (IsCommandActive(command, MOVE_RIGHT))
		axis = Vector2Add(axis, (Vector2){1.0f, 0.0f});

	// Normalise Axis of Movement
	if (Vector2Length(axis) > 0.0f)
		axis = Vector2Normalize(axis);

	mediator->object->inputAxis = axis;

	// Handle Move event
	if (Vector2Length(axis) > 0.0f)
		HandleEvent(mediator->object, EVENT_MOVE, deltaTime);
	else
		HandleEvent(mediator->object, EVENT_NONE, deltaTime);

	// Jump with movement
	if (IsCommandActive(command, JUMP))
		HandleEvent(mediator->object, EVENT_JUMP, deltaTime);

	// Fire with movement
	if (IsCommandActive(command, FIRE))
		HandleEvent(mediator->object, EVENT_FIRE, deltaTime);
}

// Check if a specific command is active
bool IsCommandActive(Command command, Command filter)
{
	return (command & filter) != 0;
}

// Get the binary representation of the command as a string
// The buffer should be at least COUNT + 1 in size to hold the "string" null terminator
// Example: For command = MOVE_UP | FIRE, the output will be "100001"
void GetCommandBits(Command command, char *buffer)
{
	for (int i = COMMAND_COUNT; i >= 0; i--)
	{
		buffer[COMMAND_COUNT - i] = (command & (1 << i)) ? '1' : '0';
	}
	buffer[COMMAND_COUNT] = '\0'; // null terminator
}
