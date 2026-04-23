#include "entity.h"
#include "game.h"
#include <stdlib.h>
#include <stdio.h>

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
    .friction = 800.0f,
    .jumpForce = -400.0f,
    //.isOnPlatform = false,
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
}

void EntitySetPosition(Entity *entity, Vector2 position){
  entity->position.x = position.x;
  entity->position.y = position.y;
}

void EntityMove(Entity *entity, const float dt, TileMap *tileMap, Camera2D *camera){
  Vector2 move = {0};
  
  // check if player is alive
  if(entity->isAlive){
    if(IsKeyDown(KEY_D)){
      entity->battery -= 1 * dt;
      move.x = 1;
    }
    if(IsKeyDown(KEY_A)){ 
      entity->battery -= 1 * dt;
      move.x = -1;
    }

    // JUMP
    if(IsKeyPressed(KEY_SPACE) && entity->isGround){
      entity->battery -= 2 * dt;
      entity->velocity.y = entity->jumpForce; 
    }
  } 

  // update velocity
  entity->velocity.x += entity->acceleration * move.x * dt;
  entity->velocity.y += GRAVITY * dt;
  entity->isGround = false;

  if(!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)){
    if(entity->velocity.x > 0){
      entity->velocity.x -= entity->friction * dt;
      if(entity->velocity.x < 0) entity->velocity.x = 0;
    } else if(entity->velocity.x < 0){
      entity->velocity.x += entity->friction * dt;
      if(entity->velocity.x > 0) entity->velocity.x = 0;
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
    tileX = GetTileCollide(desiredPosition, tileMap, &points); 

  // apply player's x position
  if(!tileX){
    entity->position.x = desiredPosition.x;
  } else { 
    
    // check if not visible 
    //if(!tileX->visible){
    //  tileX->visible = true;
    //}

    switch(tileX->tileType){
      case TILE_MOVE:{
        if(tileX->move){
          entity->position.x += tileX->velocity.x * dt;
          entity->velocity.x = 0;
        }
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
  }

  // update player's y position 
  desiredPosition.x = entity->position.x;
  desiredPosition.y = entity->position.y + entity->velocity.y * dt;
 
  if(entity->isAlive)
    tileY = GetTileCollide(desiredPosition, tileMap, &points);

  // TODO: check player's y collision
  // upply player's y position
  if(!tileY){
    entity->position.y = desiredPosition.y;
  } else { 

    // check if not visible
    //if(!tileY->visible){
    //  tileY->visible = true;
    //}  

    if(entity->velocity.y > PLAYER_HEIGHT*2){
      //printf("velocity y %f\n", entity->velocity.y);
      entity->frameCounter = 0;
      entity->currentFrame = 0;
      entity->textureRec.y = 208;
      entity->velocity.y = 0;
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

    //entity->velocity.y = 0;
  }
}

void EntityDraw(Entity *entity, Texture2D texture){
  DrawTextureRec(texture, (Rectangle){0, entity->batteryDisplay, 48, 8}, (Vector2){entity->position.x, entity->position.y - 16.0f}, WHITE);
  DrawTextureRec(texture, entity->textureRec, entity->position, WHITE);
}
