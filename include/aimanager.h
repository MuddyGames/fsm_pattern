#ifndef AI_MANAGER_H
#define AI_MANAGER_H

#include "command.h"

void InitAIManager();
Command PollAI();
void DeleteAIManager();

#endif // AI_MANAGER_H