#ifndef EVENTS_H
#define EVENTS_H

// Define an enumeration for different event types
typedef enum
{
    EVENT_NONE,            // No event
    EVENT_MOVE,            // Move event
    EVENT_FIRE,            // Fire event
    EVENT_JUMP,            // Jump event
    EVENT_DIE,             // Die event
    EVENT_RESPAWN,         // Respawn event
    EVENT_COLLISION_START, // Collision start event
    EVENT_COLLISION_END,   // Collision end event
    EVENT_COUNT            // Total number of events (for counting purposes)
} Event;                   // Define 'Event' as the type of the enum

#endif