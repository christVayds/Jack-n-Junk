#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include <stdint.h>
#include "map.h"

typedef enum{
  ENTITY_RUN,
  ENTITY_IDLE,
  ENTITY_JUMP,
  ENTITY_LAND,
  ENTITY_LOW,
  ENTITY_HIT
} EntityState;

typedef struct{
  EntityState entityState;
  Vector2 position;
  Rectangle textureRec;
  Vector2 velocity;
  float acceleration;
  float maxSpeed;
  float friction; 
  bool isGround;
  float jumpForce;
  int32_t life;       // max 3 
  bool isAlive;
  float battery;    // entity battery life
  int32_t batteryDisplay;
  int32_t level;
  int32_t score;
  int32_t currentFrame;
  int32_t frameCounter;
} Entity;

Entity EntityNew(Vector2 position);
void EntitySetPosition(Entity *entity, Vector2 position);
void EntityUpdate(Entity *entity);
void EntityMove(Entity *entity, const float dt, TileMap *tileMap, Camera2D *camera, Sound *sounds);
void EntityDraw(Entity *entity, Texture2D texture);

#endif
