#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include <stdint.h>
#include "map.h"

typedef struct{
  Vector2 position;
  Rectangle textureRec;
  Vector2 velocity;
  float acceleration;
  float maxSpeed;
  float friction; 
  bool isGround;
  float jumpForce;
  int32_t life;
} Entity;

Entity EntityNew(Vector2 position, Rectangle textureRec, int32_t life);
void EntitySetPosition(Entity *entity, Vector2 position);
void EntityMove(Entity *entity, const float dt, TileMap *tileMap, Camera2D *camera);
void EntityDraw(Entity *entity);

#endif
