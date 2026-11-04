#include "game.h"

// Initialise Game Data
void InitGame(GameData *data)
{
	// Initialise a player object with the name "Player Hero"
	float x_position = SCREEN_WIDTH / 2;
	float y_position = SCREEN_HEIGHT / 2;
	int radius = DEFAULT_GAMEOBJECT_RADIUS;

	Player *player = InitPlayer("Player Hero", x_position, y_position, radius);
	data->player = player;
	data->player->base.direction = (Vector2){0.0f, 0.0f};

	// Create Mediator
	data->playerMediator = CreateMediator((GameObject *)data->player);

	// Initialise an NPC object with the name "NPC Guard"
	NPC *enemy = InitNPC("NPC Guard", 100.0f, 200.0f, radius);

	data->npc = enemy;
	data->npc->base.direction = (Vector2){0.0f, 0.0f};

	// Create Mediator
	data->npcMediator = CreateMediator((GameObject *)data->npc);

	// Initialise Input Managers
	InitInputManager();
	InitAIManager();
}

// Update Game Data
void UpdateGame(GameData *data, float deltaTime)
{
	// Poll input and Handle Command
	Command command = PollInput();
	MediatorHandleCommand(data->playerMediator, command, deltaTime);
	UpdateState((GameObject *)data->player, deltaTime);

	Command npcCommand = PollAI();
	MediatorHandleCommand(data->npcMediator, npcCommand, deltaTime);
	UpdateState((GameObject *)data->npc, deltaTime);

	// Update the colliders, Collisions using cute_c2 Circle collider
	data->player->base.collider.p.x = data->player->base.x;
	data->player->base.collider.p.y = data->player->base.y;
	data->player->base.collider.r = data->player->base.r;

	data->npc->base.collider.p.x = data->npc->base.x;
	data->npc->base.collider.p.y = data->npc->base.y;
	data->npc->base.collider.r = data->npc->base.r;

	bool isColliding = CheckCollision(&data->player->base, &data->npc->base);

	// ENTER Collision
	if (isColliding && !data->player->base.isColliding)
	{
		// Notify FSMs
		HandleEvent(&data->player->base, EVENT_COLLISION_START, deltaTime);
		HandleEvent(&data->npc->base, EVENT_COLLISION_START, deltaTime);

		// Respond to Collision
		CollisionEntry(&data->player->base, &data->npc->base);
		HandleCollision(&data->player->base, &data->npc->base);
	}
	else if (!isColliding && data->player->base.isColliding)
	{
		// EXIT Collision
		HandleEvent(&data->player->base, EVENT_COLLISION_END, deltaTime);
		HandleEvent(&data->npc->base, EVENT_COLLISION_END, deltaTime);

		CollisionExit(&data->player->base, &data->npc->base);
	}
	else if (isColliding && data->player->base.isColliding)
	{
		// ONGOING Collision
		HandleCollision(&data->player->base, &data->npc->base);
	}

	data->player->base.isColliding = isColliding;
	data->npc->base.isColliding = isColliding;
}

// Draw GameObject HealthBar
void DrawHealthBar(const GameObject *object)
{
	// Drawing Player Health Bar
	const int healthBarWidth = 100;
	const int healthBarHeight = 10;
	const int healthBarX = object->x - (healthBarWidth / 2); // X position of health bar centred over player
	const int healthBarY = object->y - 40;					 // Y position of health bar over player

	// Calculate health percentage
	float healthPercentage = (float)object->health / 100;

	// Draw the health bar background (red)
	DrawRectangle(healthBarX, healthBarY, healthBarWidth, healthBarHeight, GRAY);

	// Draw the health bar foreground (green, based on current health)
	DrawRectangle(healthBarX, healthBarY, healthBarWidth * healthPercentage, healthBarHeight, GREEN);
}

// Draw the GameObject in this example a simple Circle which is a collider
void DrawGameObject(const GameObject *object)
{
	// GameObject Circle
	DrawCircle(
		object->x,
		object->y,
		object->r,
		object->color);
}

// Draw position information at the bottom of the GameObject
void DrawPositionInformation(const char *text, const GameObject *object)
{
	DrawText(
		text,
		object->x - (MeasureText(text, 20) / 2), // Measure the text with with MeasureText
		object->y + 30,							 // Place it below the GameObject
		20, DARKBLUE);
}

// Draw Game Data
void DrawGame(const GameData *data)
{
	// Drawing Player and Position Data
	const char *text = TextFormat(
		"(%.f, %.f)",
		data->player->base.x,
		data->player->base.y);

	// Drawing Player and Position Data
	// Player Circle
	DrawGameObject(&data->player->base);

	// Draw player healthBar
	DrawHealthBar(&data->player->base);

	// Player Information
	DrawPositionInformation(text, &data->player->base);

	// Drawing NPC and Position Data
	text = TextFormat(
		"(%.f, %.f)",
		data->npc->base.x,
		data->npc->base.y);

	// Draw NPC
	DrawGameObject(&data->npc->base);

	// Draw npc healthBar
	DrawHealthBar(&data->npc->base);

	// Draw Position
	DrawPositionInformation(text, &data->npc->base);
}

// Close Game and free resources
void CloseGame(GameData *data)
{
	printf("Game Closed!\n");

	// Free Mediators
	DeleteMediator(data->playerMediator);
	DeleteMediator(data->npcMediator);

	// Free GameObject
	DeleteGameObject((GameObject *)data->player);
	DeleteGameObject((GameObject *)data->npc);

	// Free GameData
	free(data); // Free game data memory
}