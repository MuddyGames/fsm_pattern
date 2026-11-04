#include <stdlib.h>
#include <time.h>

#include "inputmanager.h"

void InitAIManager()
{
    // Initialize AI
}

Command PollAI()
{
    // Return a random command
    int bit = rand() % COMMAND_COUNT;
    return (Command)(1 << bit);
}

void DeleteAIManager()
{
    // Cleanup resources if needed
}
