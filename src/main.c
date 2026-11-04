#include <stdio.h>

#include "game.h"
#include "constants.h"

// Function Prototypes
// Game Loop Function
void GameLoop(GameData *data);

// Main Entry Point
int main(void)
{
	// Init raylib window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Command Pattern and FSM Starter Kit GPP I");

	// Create GameData Pointer
	GameData *data = (GameData *)(malloc(sizeof(GameData)));

	// Initialise Game
	InitGame(data);

	// Set Target FPS
	SetTargetFPS(TARGET_FPS);

	// Raylib while loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Call GameLoop
		GameLoop(data);
	}

	// Free resources
	CloseGame(data);

	// Close Raylib Window
	CloseWindow();

	return 0;
}

void GameLoop(GameData *data)
{
	// Update Game Data
	// Should be outside BeginDrawing(); and EndDrawing();
	float deltaTime = GetFrameTime(); // Get delta time frame time expressed in seconds

	UpdateGame(data, deltaTime);

	// Raylib function Draw
	BeginDrawing();

	// Clear the Frame
	ClearBackground(RAYWHITE);

	// Draw the Game Objects
	DrawGame(data);

	// Raylib End drawing to Frame Buffer
	EndDrawing();
}
