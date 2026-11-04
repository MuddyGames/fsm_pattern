#include "player.h"

Player *InitPlayer(const char *name, float x, float y, int r)
{
	Player *player = (Player *)malloc(sizeof(Player));
	if (!player)
	{
		fprintf(stderr, "Failed to allocate player\n");
		exit(1);
	}

	InitGameObject(&player->base, name);

	// Set Position
	player->base.x = x;
	player->base.y = y;
	player->base.r = r;
	player->base.color = GRAY;

	player->stamina = 100.0f;
	player->mana = 100.0f;

	InitPlayerFSM(&player->base);

	return player;
}

void InitPlayerFSM(GameObject *object)
{
	object->stateConfigs = (StateConfig *)malloc(sizeof(StateConfig) * STATE_COUNT);
	if (!object->stateConfigs)
	{
		fprintf(stderr, "Failed to allocate state configs\n");
		exit(1);
	}

	// ---- STATE_IDLE state configuration ----
	// Define valid transitions from STATE_IDLE
	State idleValidTransitions[] = {STATE_MOVING, STATE_FIRING, STATE_JUMPING, STATE_DEAD};

	// Set up the state configuration for STATE_IDLE
	object->stateConfigs[STATE_IDLE].name = "Player_Idle";
	object->stateConfigs[STATE_IDLE].HandleEvent = PlayerIdleHandleEvent;
	object->stateConfigs[STATE_IDLE].Entry = PlayerEnterIdle;
	object->stateConfigs[STATE_IDLE].Update = PlayerUpdateIdle;
	object->stateConfigs[STATE_IDLE].Exit = PlayerExitIdle;

	// Configure valid transitions for STATE_IDLE
	StateTransitions(&object->stateConfigs[STATE_IDLE], idleValidTransitions, sizeof(idleValidTransitions) / sizeof(State));

	// ---- STATE_MOVING state configuration ----
	// Define valid transitions from STATE_MOVING
	State movingValidTransitions[] = {STATE_IDLE, STATE_FIRING, STATE_DEAD};

	// Set up the state configuration for STATE_MOVING
	object->stateConfigs[STATE_MOVING].name = "Player_Moving";
	object->stateConfigs[STATE_MOVING].HandleEvent = PlayerMovingHandleEvent;
	object->stateConfigs[STATE_MOVING].Entry = PlayerEnterMoving;
	object->stateConfigs[STATE_MOVING].Update = PlayerUpdateMoving;
	object->stateConfigs[STATE_MOVING].Exit = PlayerExitMoving;

	// Configure valid transitions for STATE_MOVING
	StateTransitions(&object->stateConfigs[STATE_MOVING], movingValidTransitions, sizeof(movingValidTransitions) / sizeof(State));

	// ---- STATE_FIRING state configuration ----
	// Define valid transitions from STATE_FIRING
	State firingValidTransitions[] = {STATE_IDLE, STATE_MOVING, STATE_DEAD};

	// Set up the state configuration for STATE_FIRING
	object->stateConfigs[STATE_FIRING].name = "Player_Firing";
	object->stateConfigs[STATE_FIRING].HandleEvent = PlayerFiringHandleEvent;
	object->stateConfigs[STATE_FIRING].Entry = PlayerEnterFiring;
	object->stateConfigs[STATE_FIRING].Update = PlayerUpdateFiring;
	object->stateConfigs[STATE_FIRING].Exit = PlayerExitFiring;

	// Configure valid transitions for STATE_FIRING
	StateTransitions(&object->stateConfigs[STATE_FIRING], firingValidTransitions, sizeof(firingValidTransitions) / sizeof(State));

	// ---- STATE_JUMPING state configuration ----
	// Define valid transitions from STATE_JUMPING
	State jumpingValidTransitions[] = {STATE_IDLE, STATE_FIRING, STATE_DEAD};

	// Set up the state configuration for STATE_JUMPING
	object->stateConfigs[STATE_JUMPING].name = "Player_Jumping";
	object->stateConfigs[STATE_JUMPING].HandleEvent = PlayerJumpingHandleEvent;
	object->stateConfigs[STATE_JUMPING].Entry = PlayerEnterJumping;
	object->stateConfigs[STATE_JUMPING].Update = PlayerUpdateJumping;
	object->stateConfigs[STATE_JUMPING].Exit = PlayerExitJumping;

	// Configure valid transitions for STATE_JUMPING
	StateTransitions(&object->stateConfigs[STATE_JUMPING], jumpingValidTransitions, sizeof(jumpingValidTransitions) / sizeof(State));

	// ---- STATE_DEAD state configuration ----
	// Define valid transitions from STATE_DEAD
	State deadValidTransitions[] = {STATE_RESPAWN};

	// Set up the state configuration for STATE_DEAD
	object->stateConfigs[STATE_DEAD].name = "Player_Dead";
	object->stateConfigs[STATE_DEAD].HandleEvent = PlayerDieHandleEvent;
	object->stateConfigs[STATE_DEAD].Entry = PlayerEnterDie;
	object->stateConfigs[STATE_DEAD].Update = PlayerUpdateDie;
	object->stateConfigs[STATE_DEAD].Exit = PlayerExitDie;

	// Configure valid transitions for STATE_DEAD
	StateTransitions(&object->stateConfigs[STATE_DEAD], deadValidTransitions, sizeof(deadValidTransitions) / sizeof(State));

	// ---- STATE_RESPAWN state configuration ----
	// Define valid transitions from STATE_RESPAWN
	State respawnValidTransitions[] = {STATE_IDLE};

	// Set up the state configuration for STATE_RESPAWN
	object->stateConfigs[STATE_RESPAWN].name = "Player_Respawn";
	object->stateConfigs[STATE_RESPAWN].HandleEvent = PlayerRespawnHandleEvent;
	object->stateConfigs[STATE_RESPAWN].Entry = PlayerEnterRespawn;
	object->stateConfigs[STATE_RESPAWN].Update = PlayerUpdateRespawn;
	object->stateConfigs[STATE_RESPAWN].Exit = PlayerExitRespawn;

	// Configure valid transitions for STATE_RESPAWN
	StateTransitions(&object->stateConfigs[STATE_RESPAWN], respawnValidTransitions, sizeof(respawnValidTransitions) / sizeof(State));
}

// Handles events for the Player when in the Idle state
void PlayerIdleHandleEvent(GameObject *object, Event event, float deltaTime)
{
	Player *player = (Player *)object;
	printf("\n%s Idle HandleEvent\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);

	switch (event)
	{
	case EVENT_MOVE:
		ChangeState(object, STATE_MOVING, deltaTime);
		break;
	case EVENT_FIRE:
		ChangeState(object, STATE_FIRING, deltaTime);
		break;
	case EVENT_JUMP:
		ChangeState(object, STATE_JUMPING, deltaTime);
		break;
	case EVENT_DIE:
		ChangeState(object, STATE_DEAD, deltaTime);
		break;
	case EVENT_NONE:
		object->previousState = object->currentState;
		break;
	// Ignore Events for other cases
	case EVENT_RESPAWN:
	case EVENT_COLLISION_START:
	case EVENT_COLLISION_END:
	case EVENT_COUNT:
		break;
	}
}

// Handles events for the Player when in the moving state
void PlayerMovingHandleEvent(GameObject *object, Event event, float deltaTime)
{
	Player *player = (Player *)object;
	printf("%s moving HandleEvent\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);

	switch (event)
	{
	case EVENT_NONE:
		ChangeState(object, STATE_IDLE, deltaTime);
		break;
	case EVENT_JUMP:
		ChangeState(object, STATE_JUMPING, deltaTime);
		break;
	case EVENT_FIRE:
		ChangeState(object, STATE_FIRING, deltaTime);
		break;
	case EVENT_DIE:
		ChangeState(object, STATE_DEAD, deltaTime);
		break;
	// Ignore Events for other cases
	case EVENT_MOVE:
	case EVENT_RESPAWN:
	case EVENT_COLLISION_START:
	case EVENT_COLLISION_END:
	case EVENT_COUNT:
		break;
	}
}

// Handles events for the Player when in the Firing state
void PlayerFiringHandleEvent(GameObject *object, Event event, float deltaTime)
{
	Player *player = (Player *)object;
	printf("\n%s Firing HandleEvent\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);

	switch (event)
	{
	case EVENT_DIE:
		// Transition to Dead state if a die event is received
		ChangeState(object, STATE_DEAD, deltaTime);
		break;
	// Ignore Events for other cases
	case EVENT_NONE:
	case EVENT_MOVE:
	case EVENT_FIRE:
	case EVENT_JUMP:
	case EVENT_RESPAWN:
	case EVENT_COLLISION_START:
	case EVENT_COLLISION_END:
	case EVENT_COUNT:
		break;
	}
}

// Handles events for the Player when in the Jumping  state
void PlayerJumpingHandleEvent(GameObject *object, Event event, float deltaTime)
{
	Player *player = (Player *)object;
	printf("\n%s Jumping HandleEvent\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);

	switch (event)
	{
	case EVENT_DIE:
		// Transition to Dead state if a die event is received
		ChangeState(object, STATE_DEAD, deltaTime);
		break;
	// Ignore Events for other cases
	case EVENT_NONE:
	case EVENT_MOVE:
	case EVENT_FIRE:
	case EVENT_JUMP:
	case EVENT_RESPAWN:
	case EVENT_COLLISION_START:
	case EVENT_COLLISION_END:
	case EVENT_COUNT:
		break;
	}
}

// Handles events for the Player when in the Die state
void PlayerDieHandleEvent(GameObject *object, Event event, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s Die HandleEvent\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	(void)event; // ignoring event
}

// Handles events for the Player when in the Respawn state
void PlayerRespawnHandleEvent(GameObject *object, Event event, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s Respawn HandleEvent\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	(void)event; // ignoring event
}

void PlayerEnterIdle(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s -> ENTER -> Idle\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	player->base.timer = 0.0f;
	player->base.color = DARKGREEN;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}

void PlayerUpdateIdle(GameObject *object, float deltaTime)
{
	Player *player = (Player *)object;
	printf("\n%s -> UPDATE -> Idle\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method

	player->base.timer += deltaTime;

	// Make Player Breath
	float wave = sinf(player->base.timer * 4.0f);
	float scale = 1.0f + (wave * 0.05f);

	player->base.r = (int)(DEFAULT_GAMEOBJECT_RADIUS * scale);
}

void PlayerExitIdle(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s <- EXIT <- Idle\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
	player->base.timer = 0.0f;
}

void PlayerEnterMoving(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s -> ENTER -> Moving\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	player->base.timer = 0.0f;
	player->base.color = GREEN;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}

void PlayerUpdateMoving(GameObject *object, float deltaTime)
{
	Player *player = (Player *)object;
	printf("\n%s -> UPDATE -> Moving\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method

	float speed = 200.0f;

	// Use the inputAxis set by ExecuteCommand
	player->base.direction = player->base.inputAxis;

	player->base.x += player->base.direction.x * speed * deltaTime;
	player->base.y += player->base.direction.y * speed * deltaTime;

	player->base.timer += deltaTime;

	// Make Player Breath
	float wave = sinf(player->base.timer * 3.0f);
	float scale = 1.0f + (wave * 0.05f);

	player->base.r = (int)(DEFAULT_GAMEOBJECT_RADIUS * scale);

	// Hold on screen
	ClampGameObjectOnScreen(object);
}

void PlayerExitMoving(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s <- EXIT <- Moving\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	player->base.timer = 0.0f;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}

void PlayerEnterFiring(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s -> ENTER -> Firing\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	// Example: Deduct some stamina when Firing
	player->base.timer = 0.0f;
	player->base.color = ORANGE;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}

void PlayerUpdateFiring(GameObject *object, float deltaTime)
{
	Player *player = (Player *)object;
	printf("\n%s -> UPDATE -> Firing\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	// Check if the firing should end or be interrupted (e.g., stamina depletion)
	player->base.timer += deltaTime;
	float speed = 200.0f;

	// Continue moving is inputAxis is set
	player->base.direction = player->base.inputAxis;

	player->base.x += player->base.direction.x * speed * deltaTime;
	player->base.y += player->base.direction.y * speed * deltaTime;

	// Transition to IDLE if 0.15f elapsed
	if (player->base.timer >= 0.15f)
	{
		if (Vector2Length(player->base.inputAxis) > 0.0f)
		{
			// Go back to Moving if STATE is still receiving an inputAxis
			ChangeState(object, STATE_MOVING, deltaTime);
		}
		else
		{
			// Go Idle as Firing time has elapsed
			ChangeState(object, STATE_IDLE, deltaTime);
		}
	}

	// Hold on screen
	ClampGameObjectOnScreen(object);
}

void PlayerExitFiring(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s <- EXIT <- Firing\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	// Reset or adjust any temporary changes during firing, if needed
	player->base.timer = 0.0f;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}

void PlayerEnterJumping(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s -> ENTER -> Jumping\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	// Example: Deduct some stamina for Jumping
	player->base.timer = 0.0f;
	player->base.color = SKYBLUE;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;

	if (Vector2Length(object->inputAxis) == 0.0f)
		player->base.direction = (Vector2){0.0f, -1.0f}; // default up
	else
		player->base.direction = player->base.inputAxis;
}
void PlayerUpdateJumping(GameObject *object, float deltaTime)
{
	Player *player = (Player *)object;
	printf("\n%s -> UPDATE -> Jumping\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	// Example: Check if the Jumping duration is over

	float speed = 300.0f;	// units per second
	float duration = 0.25f; // jump lasts 0.25 seconds

	player->base.timer += deltaTime;

	// Move in the current direction each frame
	player->base.x += player->base.direction.x * speed * deltaTime;
	player->base.y += player->base.direction.y * speed * deltaTime;

	// When timer exceeds duration, go back to idle
	if (player->base.timer >= duration)
	{
		ChangeState(object, STATE_IDLE, deltaTime);
	}

	// Hold on screen
	ClampGameObjectOnScreen(object);
}

void PlayerExitJumping(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s <- EXIT <- Jumping\n", object->name);
	printf("Stamina: %.1f, Mana: %.1f\n\n", player->stamina, player->mana);
	// Complete the remainder of the method
	// Reset any temporary Jumping effects if necessary
	// Reset color if you want
	player->base.timer = 0.0f;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}

void PlayerEnterDie(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s -> ENTER -> Die\n", object->name);
	// Complete the remainder of the method
	player->base.timer = 0.0f;
	player->base.color = MAROON;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}

void PlayerUpdateDie(GameObject *object, float deltaTime)
{
	printf("\n%s -> UPDATE -> Die\n", object->name);
	ChangeState(object, STATE_RESPAWN, deltaTime);
	// Complete the remainder of the method
}

void PlayerExitDie(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s <- EXIT <- Die\n", object->name);
	// Complete the remainder of the method
	player->base.timer = 0.0f;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}

void PlayerEnterRespawn(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s -> ENTER -> Respawn\n", object->name);
	// Complete the remainder of the method
	player->base.timer = 0.0f;
	player->base.color = PURPLE;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}

void PlayerUpdateRespawn(GameObject *object, float deltaTime)
{
	printf("\n%s -> UPDATE -> Respawn\n", object->name);
	ChangeState(object, STATE_IDLE, deltaTime);
	// Complete the remainder of the method
}

void PlayerExitRespawn(GameObject *object, float deltaTime)
{
	(void)deltaTime;
	Player *player = (Player *)object;
	printf("\n%s <- EXIT <- Respawn\n", object->name);
	// Complete the remainder of the method
	player->base.timer = 0.0f;
	player->base.r = DEFAULT_GAMEOBJECT_RADIUS;
}