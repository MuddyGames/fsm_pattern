#include <stdlib.h>
#include <time.h>

#include <raylib.h>

#include "inputmanager.h"

// Sample input initialization
void InitInputManager()
{
    // Initialize input sources (keyboard, controller, etc.)
}

// Sample input polling
Command PollInput()
{
    // Check keyboard input

    Command command = NONE;

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        command |= MOVE_UP;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        command |= MOVE_DOWN;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        command |= MOVE_LEFT;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        command |= MOVE_RIGHT;
    if (IsKeyPressed(KEY_SPACE))
        command |= JUMP;
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        command |= FIRE;

    TraceLog(LOG_INFO, "Keyboard Command %d", command);

    // Controller Input
    if (IsGamepadAvailable(0))
    {
        if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
            command |= MOVE_UP;
        if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
            command |= MOVE_DOWN;
        if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
            command |= MOVE_LEFT;
        if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
            command |= MOVE_RIGHT;

        if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
            command |= JUMP;
        if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_TRIGGER) > 0.5f)
            command |= FIRE;

        TraceLog(LOG_INFO, "Controller Command %d", command);
    }

    return command;
}

void DeleteInputManager()
{
    // Cleanup resources if needed
}

