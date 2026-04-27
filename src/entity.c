#include "entity.h"
#include "game.h"
#include <stdlib.h>
#include <stdio.h>

//float stepTimer = 0.0f;
//float stepDelay = 0.1f;

static void EntityAnimate(Entity *entity){
  entity->frameCounter++;
  
  if(entity->frameCounter >= (MAXFPS / FRAMESPEED)){
    entity->frameCounter = 0;
    entity->currentFrame++;
    
    // frame end 
    if(entity->currentFrame > 3){
      // check for landing animation 
      if(entity->textureRec.y == 208) entity->textureRec.y = 112;
      entity->currentFrame = 0;
    }
    entity->textureRec.x = 32 + entity->currentFrame * PLAYER_WIDTH;
  }

  // check velocity direction
  if(!entity->isAlive) return;
  if(entity->velocity.x){
    if(entity->velocity.x > 0)
      entity->textureRec.width = PLAYER_WIDTH;
    else if(entity->velocity.x < 0)
      entity->textureRec.width = -PLAYER_WIDTH;
    
    // check for landing animation
    if(entity->textureRec.y != 208) entity->textureRec.y = 112;
  }
  else 
    // check for landing animation 
    if(entity->textureRec.y != 208) entity->textureRec.y = 160;
}

Entity EntityNew(Vector2 position){
  Entity newEntity = {
    .entityState = ENTITY_IDLE,
    .position = position,
    .textureRec = (Rectangle){33, 112, PLAYER_WIDTH, PLAYER_HEIGHT},
    .life = 3,
    .isAlive = true,
    .battery = 100,
    .batteryDisplay = 464,
    .acceleration = 600.0f,
    .velocity = {0},
    .isGround = false,
    .maxSpeed = 300.0f,
    .friction = 200.0f,
    .jumpForce = -400.0f,
    .level = 0,
    .score = 0,
    .currentFrame = 0,
    .frameCounter = 0
  };

  return newEntity;
}

void EntityUpdate(Entity *entity){
  EntityAnimate(entity);

  // update battery
  if(entity->battery <= 0) entity->batteryDisplay = 512;
  else if(entity->battery <= 10) entity->batteryDisplay = 504;
  else if(entity->battery <= 20) entity->batteryDisplay = 496;
  else if(entity->battery <= 45) entity->batteryDisplay = 488;
  else if(entity->battery <= 60) entity->batteryDisplay = 480;
  else if(entity->battery <= 80) entity->batteryDisplay = 472;
  else if(entity->battery <= 100) entity->batteryDisplay = 464;
  
  /*
  switch(entity->entityState){
    case ENTITY_RUN:
      stepTimer += GetFrameTime();
      
      if(!entity->isGround){
        stepTimer = 0;
        return;
      }

      if(stepTimer >= stepDelay){ 
        PlaySound(sounds[3]);
        stepTimer = 0;
      }
      break;
    default:
      stepTimer = 0;
      break;
  } */
}

void EntitySetPosition(Entity *entity, Vector2 position){
  entity->position.x = position.x;
  entity->position.y = position.y;
}

void EntityMove(Entity *entity, const float dt, TileMap *tileMap, Camera2D *camera, Sound *sounds){
  Vector2 move = {0};
  
  // check if player is alive
  if(entity->isAlive){
    if(IsKeyDown(KEY_D)){
      entity->battery -= 1 * dt;
      move.x = 1;
      entity->entityState = ENTITY_RUN;
    }
    if(IsKeyDown(KEY_A)){ 
      entity->battery -= 1 * dt;
      move.x = -1;
      entity->entityState = ENTITY_RUN;
    }

    // JUMP
    if(IsKeyPressed(KEY_SPACE) && entity->isGround){
      entity->battery -= 2 * dt;
      entity->velocity.y = entity->jumpForce;
      entity->entityState = ENTITY_JUMP;
      PlaySound(sounds[0]);
    }
  } 

  // update velocity
  entity->velocity.x += entity->acceleration * move.x * dt;
  entity->velocity.y += GRAVITY * dt;
  entity->isGround = false;

  if(!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)){
    if(entity->velocity.x > 0){
      entity->velocity.x -= entity->friction * dt;
      if(entity->velocity.x < 0){
        entity->velocity.x = 0;
        entity->entityState = ENTITY_IDLE;
      }
    } else if(entity->velocity.x < 0){
      entity->velocity.x += entity->friction * dt;
      if(entity->velocity.x > 0){
        entity->velocity.x = 0;
        entity->entityState = ENTITY_IDLE;
      }
    } 
  }

  // clamp speed 
  if(entity->velocity.x > entity->maxSpeed) entity->velocity.x = entity->maxSpeed;
  if(entity->velocity.x < -entity->maxSpeed) entity->velocity.x = -entity->maxSpeed;

  Rectangle desiredPosition = {.x=entity->position.x, .y=entity->position.y, .width=PLAYER_WIDTH, .height=PLAYER_HEIGHT};
  desiredPosition.x += entity->velocity.x * dt;

  Tile *tileX = NULL;
  Tile *tileY = NULL; 

  WorldPoints points = BlocksVisibleInCamera(camera);

  // TODO: check x collision
  if(entity->isAlive)
    tileX = GetTileCollide(desiredPosition, tileMap, &points, &entity->level); 

  // apply player's x position
  if(!tileX){
    entity->position.x = desiredPosition.x;
  } else {  
    switch(tileX->tileType){
      case TILE_MOVE:{
        entity->isAlive = false;
        entity->position.x += tileX->velocity.x * dt;
        entity->velocity.x = 0;
        break;
      }
      case TILE_DEADLY:{
        entity->isAlive = false;
        break;
      }
      default:{
        // snap player position
        if(entity->velocity.x > 0)
          entity->position.x = tileX->position.x - PLAYER_WIDTH;
        else if(entity->velocity.x < 0)
          entity->position.x = tileX->position.x + TILESIZE;
        entity->velocity.x = 0;
      }
    }
    entity->entityState = ENTITY_HIT;
  }

  // update player's y position 
  desiredPosition.x = entity->position.x;
  desiredPosition.y = entity->position.y + entity->velocity.y * dt;
 
  if(entity->isAlive)
    tileY = GetTileCollide(desiredPosition, tileMap, &points, &entity->level);

  // TODO: check player's y collision
  // upply player's y position
  if(!tileY){
    entity->position.y = desiredPosition.y;
  } else { 

    // landing
    if(entity->velocity.y > PLAYER_HEIGHT*3){
      entity->frameCounter = 0;
      entity->currentFrame = 0;
      entity->textureRec.y = 208;
      entity->velocity.y = 0;
      entity->entityState = ENTITY_LAND;
      PlaySound(sounds[1]);
    }

    switch(tileY->tileType){
      case TILE_MOVE:{ 
        if(tileY->move){
          entity->isGround = true;
          entity->position.x += tileY->velocity.x * dt;
          entity->position.y += tileY->velocity.y * dt;

          if(entity->velocity.y > 0){
            entity->position.y = tileY->position.y - PLAYER_HEIGHT;
          } else if(entity->velocity.y < 0){
            entity->position.y = tileY->position.y + TILESIZE;
          }
        entity->velocity.y = 0;
        }
        //printf("isGround %f\n", entity->velocity.x);
        break;
      }
      case TILE_SPRINT:{
        entity->isGround = true;
        entity->velocity.x += entity->acceleration * tileY->velocity.x;
        entity->velocity.y = 0;
        break;
      }
      case TILE_DEADLY:{
        entity->isAlive = false;
        entity->velocity.y = 0;
        break;
      }
      case TILE_MACHINE:{
        entity->velocity.y += entity->acceleration * tileY->velocity.y;
        entity->velocity.x += entity->acceleration * tileY->velocity.x;
        PlaySound(sounds[0]);
        break;
      }
      default:{ 
        // snap player position
        if(entity->velocity.y > 0){
          entity->position.y = tileY->position.y - PLAYER_HEIGHT;
          entity->isGround = true;
        } else if(entity->velocity.y < 0){
          entity->position.y = tileY->position.y + TILESIZE;
          entity->isGround = true;
        }
        entity->velocity.y = 0;
      } 
    }
  }
}

void EntityDraw(Entity *entity, Texture2D texture){
  DrawTextureRec(texture, (Rectangle){0, entity->batteryDisplay, 48, 8}, (Vector2){entity->position.x, entity->position.y - 16.0f}, WHITE);
  DrawTextureRec(texture, entity->textureRec, entity->position, WHITE);
}
