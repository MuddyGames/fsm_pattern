#include "npc.h"

// Initialize a new NPC object with a given name
NPC *InitNPC(const char *name, float x, float y, int r)
{
	// Allocate memory for the NPC structure
	NPC *npc = (NPC *)malloc(sizeof(NPC));

	// Check if memory allocation failed
	if (!npc)
	{
		// Print an error message to stderr and terminate the program if allocation fails
		fprintf(stderr, "Failed to allocate NPC\n");
		exit(1);
	}

	// Initialize the base GameObject structure within the NPC with the provided name
	InitGameObject(&npc->base, name);

	// Set Position
	npc->base.x = x;
	npc->base.y = y;
	npc->base.r = r;
	npc->base.color = GRAY;

	// Set the default aggression level for the NPC
	npc->aggression = 50;

	// Initialize the NPC's finite state machine (FSM) with state configurations
	InitNPCFSM(&npc->base);

	// Return a pointer to the initialized NPC object
	return npc;
}

// Initialize the Finite State Machine (FSM) for the NPC GameObject
void InitNPCFSM(GameObject *object)
{
	// Allocate memory for the state configurations array with a size for all possible states
	object->stateConfigs = (StateConfig *)malloc(sizeof(StateConfig) * STATE_COUNT);

	// Check if memory allocation for state configurations failed
	if (!object->stateConfigs)
	{
		// Print an error message and exit if allocation fails
		fprintf(stderr, "Failed to allocate state configs\n");
		exit(1);
	}

	// ---- STATE_IDLE state configuration ----
	// Define valid transitions from STATE_IDLE
	State idleValidTransitions[] = {STATE_FIRING, STATE_DEAD};

	// Set up the state configuration for STATE_IDLE
	object->stateConfigs[STATE_IDLE].name = "NPC_Idle";
	object->stateConfigs[STATE_IDLE].HandleEvent = NPCIdleHandleEvent;
	object->stateConfigs[STATE_IDLE].Entry = NPCEnterIdle;
	object->stateConfigs[STATE_IDLE].Update = NPCUpdateIdle;
	object->stateConfigs[STATE_IDLE].Exit = NPCExitIdle;

	// Configure valid transitions for STATE_IDLE
	StateTransitions(&object->stateConfigs[STATE_IDLE], idleValidTransitions, sizeof(idleValidTransitions) / sizeof(State));

	// ---- STATE_FIRING state configuration ----
	// Define valid transitions from STATE_FIRING
	State firingValidTransitions[] = {STATE_IDLE, STATE_DEAD};

	// Set up the state configuration for STATE_FIRING
	object->stateConfigs[STATE_FIRING].name = "NPC_Firing";
	object->stateConfigs[STATE_FIRING].HandleEvent = NPCFiringHandleEvent;
	object->stateConfigs[STATE_FIRING].Entry = NPCEnterFiring;
	object->stateConfigs[STATE_FIRING].Update = NPCUpdateFiring;
	object->stateConfigs[STATE_FIRING].Exit = NPCExitFiring;

	// Configure valid transitions for STATE_FIRING
	StateTransitions(&object->stateConfigs[STATE_FIRING], firingValidTransitions, sizeof(firingValidTransitions) / sizeof(State));

	// ---- STATE_DEAD state configuration ----
	// Define valid transitions from STATE_DEAD
	State deadValidTransitions[] = {STATE_IDLE}; // Should go to STATE_RESPAWN to keep kit small goes to IDLE

	// Set up the state configuration for STATE_DEAD
	object->stateConfigs[STATE_DEAD].name = "NPC_Dead";
	object->stateConfigs[STATE_DEAD].HandleEvent = NPCDeadHandleEvent;
	object->stateConfigs[STATE_DEAD].Entry = NPCEnterDead;
	object->stateConfigs[STATE_DEAD].Update = NPCUpdateDead;
	object->stateConfigs[STATE_DEAD].Exit = NPCExitDead;

	// Configure valid transitions for STATE_DEAD
	StateTransitions(&object->stateConfigs[STATE_DEAD], deadValidTransitions, sizeof(deadValidTransitions) / sizeof(State));

	// For unimplemented states, set them to empty defaults
#define EMPTY_STATE_CONFIG \
	(StateConfig){NULL, NULL, NULL, NULL, NULL, NULL, 0}
	object->stateConfigs[STATE_JUMPING] = EMPTY_STATE_CONFIG;
	object->stateConfigs[STATE_MOVING] = EMPTY_STATE_CONFIG;
	object->stateConfigs[STATE_RESPAWN] = EMPTY_STATE_CONFIG;
}

// Handles events for the NPC when in the Idle state
void NPCIdleHandleEvent(GameObject *object, Event event, float deltaTime)
{
	NPC *npc = (NPC *)object;
	printf("\n%s Idle HandleEvent\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);

	switch (event)
	{
	case EVENT_FIRE:
		// Transition to Firing state if an fire event is received
		ChangeState(object, STATE_FIRING, deltaTime);
		break;
	case EVENT_DIE:
		// Transition to Dead state if a die event is received
		ChangeState(object, STATE_DEAD, deltaTime);
		break;
	// Ignore Events for other cases
	case EVENT_NONE:
	case EVENT_MOVE:
	case EVENT_JUMP:
	case EVENT_RESPAWN:
	case EVENT_COLLISION_START:
	case EVENT_COLLISION_END:
	case EVENT_COUNT:
		break;
	}
}

// Handles events for the NPC when in the Firing state
void NPCFiringHandleEvent(GameObject *object, Event event, float deltaTime)
{
	NPC *npc = (NPC *)object;
	printf("\n%s Firing HandleEvent\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);

	switch (event)
	{
	case EVENT_DIE:
		// Transition to Dead state if a die event is received
		ChangeState(object, STATE_DEAD, deltaTime);
		break;
	// Ignore Events for other cases
	case EVENT_NONE:
	case EVENT_MOVE:
	case EVENT_JUMP:
	case EVENT_FIRE:
	case EVENT_RESPAWN:
	case EVENT_COLLISION_START:
	case EVENT_COLLISION_END:
	case EVENT_COUNT:
		break;
	}
}

// Handles events for the NPC when in the Dead state
void NPCDeadHandleEvent(GameObject *object, Event event, float deltaTime)
{
	NPC *npc = (NPC *)object;
	printf("\n%s Dead HandleEvent\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);

	switch (event)
	{
	case EVENT_RESPAWN:
		// Transition back to Idle (e.g., after dying, NPC should stay dead until respawned)
		// Transition to Idle or another state (e.g., Spawn) upon respawn event
		// You may want to handle respawning logic in the future here.
		ChangeState(object, STATE_IDLE, deltaTime); // or STATE_SPAWNING if you have that state
		break;
	// Ignore Events for other cases (e.g., move, defend) as dead NPCs cannot perform these actions.
	case EVENT_NONE:
	case EVENT_DIE:
	case EVENT_FIRE:
	case EVENT_JUMP:
	case EVENT_MOVE:
	case EVENT_COLLISION_START:
	case EVENT_COLLISION_END:
	case EVENT_COUNT:
		break;
	}
}

// Enter function for Idle state, executed once upon entering Idle
void NPCEnterIdle(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	NPC *npc = (NPC *)object;
	printf("%s -> ENTER -> Idle\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);
	// Initialisation code for entering Idle state, such as resetting timers or animation.
	npc->base.timer = 0.0f;
	npc->base.color = DARKGRAY;
	npc->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}

// Update function for Idle state, called repeatedly during game ticks while in Idle
void NPCUpdateIdle(GameObject *object, float deltaTime)
{
	NPC *npc = (NPC *)object;
	printf("%s -> UPDATE -> Idle\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);
	// During game loop and game ticks, execute Idle state behavior here, such as patrolling or observing.

	npc->base.timer += deltaTime;
	float wave = sinf(npc->base.timer * 2.0f);
	float scale = 1.0f + wave * 0.02f;
	npc->base.r = (int)(DEFAULT_GAMEOBJECT_RADIUS * scale);
}

// Exit function for Idle state, executed once upon leaving Idle
void NPCExitIdle(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	NPC *npc = (NPC *)object;
	printf("%s <- EXIT <- Idle\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);
	// Cleanup code for leaving Idle state, if any.
}

// Enter function for Firing state, executed once upon entering Firing
void NPCEnterFiring(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	NPC *npc = (NPC *)object;
	printf("%s -> ENTER -> Firing\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);
	// Initialisation code for entering Firing state, such as setting up firing animations.
	npc->base.timer = 0.0f;
	npc->base.color = ORANGE;
}

// Update function for Firing state, called repeatedly during game ticks while in Firing
void NPCUpdateFiring(GameObject *object, float deltaTime)
{
	NPC *npc = (NPC *)object;
	printf("%s -> UPDATE -> Firing\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);
	// During game loop and game ticks, execute Firing state behavior here, such as dealing damage.
	// Transition back to IDLE
	npc->base.timer += deltaTime;

	if (npc->base.timer >= 1.00f)
	{
		ChangeState(object, STATE_IDLE, deltaTime);
		return;
	}

	// Make NPC Breath
	float wave = sinf(npc->base.timer * 3.0f);
	float scale = 1.0f + (wave * 0.05f);
	npc->base.r = (int)(DEFAULT_GAMEOBJECT_RADIUS * scale);
}

// Exit function for Firing state, executed once upon leaving Firing
void NPCExitFiring(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	NPC *npc = (NPC *)object;
	printf("%s <- EXIT <- Firing\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);
	// Cleanup code for leaving Firing state, such as resetting firing cooldown.
	npc->base.r = DEFAULT_GAMEOBJECT_RADIUS;
	npc->base.timer = 0.0f;
}

// Enter function for Dead state, executed once upon entering Dead
void NPCEnterDead(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	NPC *npc = (NPC *)object;
	printf("%s -> ENTER -> Dead\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);
	// Initialisation code for entering Dead state, such as playing death animation or disabling further actions.
	npc->base.color = BLACK;
	npc->base.r = DEFAULT_GAMEOBJECT_RADIUS;
	npc->base.timer = 0.0f;
}

// Update function for Dead state, called repeatedly during game ticks while in Dead
void NPCUpdateDead(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	NPC *npc = (NPC *)object;
	printf("%s -> UPDATE -> Dead\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);
	// During game loop and game ticks, execute Dead state behavior here, such as preventing any actions.
	// This could be a place to check if the NPC should be removed or respawned.
}

// Exit function for Dead state, executed once upon leaving Dead
void NPCExitDead(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	NPC *npc = (NPC *)object;
	printf("%s -> EXIT -> Dead\n", object->name);
	printf("Aggression: %d\n\n", npc->aggression);
	// Cleanup code for leaving Dead state, such as removing NPC from the active world, playing respawn animations, etc.
}
