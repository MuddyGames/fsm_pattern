#include "hud.h"
#include "constants.h"

// Set the HUD to its starting state
void InitHud(Hud *hud)
{
    hud->lastCommand = NONE;
    hud->jumpCount = 0;
    hud->stateName = "";
}

// React to this frame's command and state
void UpdateHud(Hud *hud, Command command, const char *stateName)
{
    hud->lastCommand = command;
    hud->stateName = stateName ? stateName : "";

    // Note: this counts JUMP commands, not jumps. The FSM decides whether the jump happens

    // JUMP is a single key press (IsKeyPressed), so this counts each jump once
    if (IsCommandActive(command, JUMP))
    {
        hud->jumpCount++;
    }
}

// Helper function: draw a command's name if its bit is set, then move down a line
static void DrawActiveCommand(Command activeCommand, Command filter, const char *commandText, int *yPositionOffset, Color color)
{
    if (IsCommandActive(activeCommand, filter))
    {
        DrawText(commandText, 10, *yPositionOffset, DEFAULT_FONT_SIZE, color);
        *yPositionOffset += DEFAULT_TEXT_Y_OFFSET;
    }
}

// Draw the HUD on screen
void DrawHud(const Hud *hud)
{
    int y = 10;

    // Command as bits, e.g. MOVE_LEFT | JUMP -> 010100
    char bits[COMMAND_COUNT + 1];
    GetCommandBits(hud->lastCommand, bits);
    DrawText(TextFormat("Command: %s", bits), 10, y, DEFAULT_FONT_SIZE, DARKGRAY);
    y += DEFAULT_TEXT_Y_OFFSET;

    // Player's current state, e.g. Player_Jumping
    DrawText(TextFormat("State: %s", hud->stateName), 10, y, DEFAULT_FONT_SIZE, DARKGRAY);
    y += DEFAULT_TEXT_Y_OFFSET;

    DrawText(TextFormat("Jumps: %d", hud->jumpCount), 10, y, DEFAULT_FONT_SIZE, DARKGRAY);
    y += DEFAULT_TEXT_Y_OFFSET;

    // Names of the active commands
    DrawActiveCommand(hud->lastCommand, MOVE_UP, "MOVE_UP", &y, BLUE);
    DrawActiveCommand(hud->lastCommand, MOVE_DOWN, "MOVE_DOWN", &y, BLUE);
    DrawActiveCommand(hud->lastCommand, MOVE_LEFT, "MOVE_LEFT", &y, BLUE);
    DrawActiveCommand(hud->lastCommand, MOVE_RIGHT, "MOVE_RIGHT", &y, BLUE);
    DrawActiveCommand(hud->lastCommand, JUMP, "JUMP", &y, DARKGREEN);
    DrawActiveCommand(hud->lastCommand, FIRE, "FIRE", &y, RED);
}
