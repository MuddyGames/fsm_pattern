#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

// Forward declaration of the Mediator struct
typedef struct Mediator Mediator;

// Define the Command enum combining different commands into bit flags
// Movement could be defined as just MOVE with direction (for simplicity, each direction is separate)

typedef enum
{
	NONE		 	= 0,		// No command
	MOVE_UP 		= 1 << 0,	// Binary: 000001
	MOVE_DOWN 		= 1 << 1,	// Binary: 000010
	MOVE_LEFT 		= 1 << 2,	// Binary: 000100
	MOVE_RIGHT 		= 1 << 3,	// Binary: 001000
	JUMP 			= 1 << 4,	// Binary: 010000
	FIRE 			= 1 << 5,	// Binary: 100000
	COMMAND_COUNT   = 6 // Total number of commands
} Command;

// Function to execute a command
void ExecuteCommand(Mediator *mediator, Command command, float deltaTime);

// Function to show active command bits (for debugging)
void GetCommandBits(Command cmd, char *buffer);

// Check if a specific command is active
bool IsCommandActive(Command command, Command filter);

#endif // COMMAND_H