#include <raylib.h>
#include <stdio.h>

#include "collision.h"

// Check for collision between Player and NPC
bool CheckCollision(GameObject *lhs, GameObject *rhs)
{
	return c2CircletoCircle(lhs->collider, rhs->collider);
}

// Handle what happens during a collision
void HandleCollision(GameObject *lhs, GameObject *rhs)
{
	c2CircletoCircleManifold(lhs->collider, rhs->collider, &rhs->manifold);

	if (rhs->manifold.count > 0)
	{
		// Simple collision response: push the lhs circle out of the NPC circle along the normal
		// rhs->manifold.n is the collision normal
		// rhs->manifold.depths[0] is the penetration depth
		float depth = rhs->manifold.depths[0];
		// Does not allow the lhs to penetrate the circle
		lhs->collider.p = c2Sub(lhs->collider.p, c2Mulvs(rhs->manifold.n, depth + PUSHBACK_DISTANCE));
	}

	// Change NPC color to indicate it has collided
	lhs->color = RED;
	rhs->color = RED;

	// Change position of Player as in respond to collision
	lhs->x = lhs->collider.p.x;
	lhs->y = lhs->collider.p.y;

}

// Collision Entry
void CollisionEntry(GameObject *lhs, GameObject *rhs)
{
	lhs->health -= DAMAGE_DEFAULT; // Example: Reduce lhs's health on collision
	rhs->health -= DAMAGE_DEFAULT;

	printf("Player Damage [ %d ] Health[ %d ]\n", DAMAGE_DEFAULT, lhs->health);
	printf("NPC Damage [ %d ] Health[ %d ]\n", DAMAGE_DEFAULT, rhs->health);
}

void CollisionExit(GameObject *lhs, GameObject *rhs)
{
	lhs->color = GREEN; // Reset Player default color
	rhs->color = BLUE;	// Reset NPC default color
}
