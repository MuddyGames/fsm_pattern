#include "fsm.h"
#include "gameobject.h"

// Handle an event for the given game object
// This will trigger the appropriate event handler based on the current state
void HandleEvent(GameObject *obj, Event event, float deltaTime)
{
    // Get the state configuration for the current state of the object
    StateConfig *config = &obj->stateConfigs[obj->currentState];

    // If a HandleEvent function is defined for this state, handle the event
    if (config->HandleEvent)
    {
        config->HandleEvent(obj, event, deltaTime); // Call the state's event handler
    }
}

// Update the game object's state (e.g., to perform animations or actions)
// This is called to execute the behavior for the current state
void UpdateState(GameObject *obj, float deltaTime)
{
    // Get the configuration for the current state
    StateConfig *config = &obj->stateConfigs[obj->currentState];

    // If an update function is defined for the current state, call it
    if (config->Update)
    {
        config->Update(obj, deltaTime); // Perform the update for the current state
    }
}

// Check if the given game object can transition to the new state
// This checks if the new state is valid according to nextStates
bool CanEnterState(GameObject *obj, State newState)
{
    // Get the current state configuration
    StateConfig *currentConfig = &obj->stateConfigs[obj->currentState];

    // Loop through the possible next states and check if newState is valid
    for (int i = 0; i < currentConfig->nextStatesCount; i++)
    {
        if (currentConfig->nextStates[i] == newState)
            return true; // Valid transition found
    }
    return false; // No valid transition found
}

// Change the state of the game object if the transition is valid
// If the transition is valid, exit the current state and enter the new state
bool ChangeState(GameObject *obj, State newState, float deltaTime)
{
    // Check if the state transition is valid
    if (!CanEnterState(obj, newState))
    {
        // If not valid, print an error message and return false
        printf("Invalid state transition from %s to %s\n",
               obj->stateConfigs[obj->currentState].name,
               obj->stateConfigs[newState].name);
        return false; // Transition failed
    }

    // Get the configuration of the current state and the new state
    StateConfig *currentConfig = &obj->stateConfigs[obj->currentState];
    StateConfig *newConfig = &obj->stateConfigs[newState];

    // Exit the current state if an exit function is defined
    if (currentConfig->Exit)
        currentConfig->Exit(obj, deltaTime);

    // Update the object's previous and current state
    obj->previousState = obj->currentState;
    obj->currentState = newState;

    // Enter the new state if an entry function is defined
    if (newConfig->Entry)
        newConfig->Entry(obj, deltaTime);

    return true; // State transition successful
}

// Helper function to initialize state transitions for a specific state
void StateTransitions(StateConfig *stateConfig, State *transitions, int count)
{
    // Allocate memory for the next states array based on count
    stateConfig->nextStates = (State *)malloc(sizeof(State) * count);
    if (!stateConfig->nextStates)
    {
        fprintf(stderr, "Failed to allocate state transitions\n");
        exit(1); // Exit if allocation fails
    }

    // Copy the transitions array into stateConfig's nextStates
    memcpy(stateConfig->nextStates, transitions, sizeof(State) * count);
    stateConfig->nextStatesCount = count; // Set the count of next states
}

// Helper function to print each state configuration's details
void PrintStateConfigs(StateConfig *stateConfigs, int stateCount)
{
    // Loop through each state configuration and print its details
    for (int i = 0; i < stateCount; i++)
    {
        StateConfig *config = &stateConfigs[i];

        // Print only if the state is properly set up (name is defined)
        if (config->name == NULL)
        {
            continue;
        }

        // Print state name and handler function availability
        printf("State: %s\n", config->name);
        printf("\tHandleEvent: %s\n", config->HandleEvent ? "Defined" : "NULL");
        printf("\tEntry: %s\n", config->Entry ? "Defined" : "NULL");
        printf("\tUpdate: %s\n", config->Update ? "Defined" : "NULL");
        printf("\tExit: %s\n", config->Exit ? "Defined" : "NULL");

        // Print the list of next possible states for this state
        printf("\tNext States: [");
        for (int j = 0; j < config->nextStatesCount; j++)
        {
            if (j > 0)
            {
                printf(", ");
            }
            printf("%d", config->nextStates[j]);
        }
        printf("]\n");
        printf("\tNext States Count: %d\n", config->nextStatesCount);
    }
}
