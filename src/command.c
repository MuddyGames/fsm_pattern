#include "command.h"
#include "fsm.h"

// Function to execute a command
// Command pattern: the player acted on the command directly (MoveLeft(), Jump()...)
// FSM pattern: the command becomes an event, and the current state decides what to do with it
void ExecuteCommand(Command command, GameObject *object, float deltaTime)
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

    // Normalise Axis of Movement (so diagonals are not faster)
    if (Vector2Length(axis) > 0.0f)
        axis = Vector2Normalize(axis);

    object->inputAxis = axis;

    // Handle Move event, or no movement (EVENT_NONE)
    if (Vector2Length(axis) > 0.0f)
        HandleEvent(object, EVENT_MOVE, deltaTime);
    else
        HandleEvent(object, EVENT_NONE, deltaTime);

    // Jump with movement
    if (IsCommandActive(command, JUMP))
        HandleEvent(object, EVENT_JUMP, deltaTime);

    // Fire with movement
    if (IsCommandActive(command, FIRE))
        HandleEvent(object, EVENT_FIRE, deltaTime);
}

// Check if a specific command is active
bool IsCommandActive(Command command, Command filter)
{
    return (command & filter) != 0;
}

// Get the binary representation of the command as a string
// The buffer should be at least COMMAND_COUNT + 1 in size to hold the "string" null terminator
// Example: For command = MOVE_UP | FIRE, the output will be "100001"
// Bits are printed from the highest (COMMAND_COUNT - 1) down to bit 0
void GetCommandBits(Command command, char *buffer)
{
    for (int i = COMMAND_COUNT - 1; i >= 0; i--)
    {
        buffer[COMMAND_COUNT - 1 - i] = (command & (1 << i)) ? '1' : '0';
    }
    buffer[COMMAND_COUNT] = '\0'; // null terminator
}
