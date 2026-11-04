#include <stdlib.h>
#include <time.h>

#include "ai_manager.h"

void InitAIManager()
{
    // Seed the random number generator so the AI is different every run
    srand((unsigned int)time(NULL));
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
