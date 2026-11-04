#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdio.h>

// Include the events header file that defines the 'Event' enum
#include "events.h"

// Forward declaration of the GameObject structure
typedef struct GameObject GameObject;

// Define function pointer types for event handling and state management
typedef void (*EventFunction)(GameObject *, Event, float); // Function type for event handlers
typedef void (*StateFunction)(GameObject *, float);        // Function type for state entry, update, and exit handlers

// Define an enumeration for different states of the game object
typedef enum
{
    STATE_IDLE,    // Idle state (no action)
    STATE_MOVING,  // Moving state
    STATE_FIRING,  // Firing state
    STATE_JUMPING, // Jumping state
    STATE_DEAD,    // Dead state (game over or defeated)
    STATE_RESPAWN, // Respawn state (respawns the player)
    STATE_COUNT    // Total number of states (for counting purposes)
} State;           // Define 'State' as the type of the enum

// Define a configuration structure for each state of the GameObject
typedef struct StateConfig
{
    const char *name;          // The name of the state (e.g., "Idle", "Moving")
    EventFunction HandleEvent; // Pointer to the function that handles events in this state
    StateFunction Entry;       // Pointer to the function that is called when entering this state
    StateFunction Update;      // Pointer to the function that is called to update the state
    StateFunction Exit;        // Pointer to the function that is called when exiting this state
    State *nextStates;         // Array of possible next states (state transitions)
    int nextStatesCount;       // Number of possible next states
} StateConfig;                 // Define 'StateConfig' as a structure that holds all state-related configurations

// Handles an event for the given game object, triggering changes in state
void HandleEvent(GameObject *obj, Event event, float deltaTime);

// Checks if the game object can enter a new state based on the current context
bool CanEnterState(GameObject *obj, State newState);

// Changes the state of the game object to the new state if possible
bool ChangeState(GameObject *obj, State newState, float deltaTime);

// Updates the current state of the game object (for example, animations, actions)
void UpdateState(GameObject *obj, float deltaTime);

// Function to initialize valid state transitions
void StateTransitions(StateConfig *stateConfig, State *transitions, int count);

// Function to print each state configuration
void PrintStateConfigs(StateConfig *stateConfigs, int stateCount);

#endif
