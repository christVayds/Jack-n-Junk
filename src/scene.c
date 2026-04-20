#include "scene.h"
#include <math.h>
#include <stdio.h>

static void UpdatePlayerCamera(Camera2D *camera, Vector2 *playerposition){
  float smoothSpeed = 0.4f;
  camera->target.x += floorf((playerposition->x - camera->target.x) * smoothSpeed);
  camera->target.y += floorf((playerposition->y - camera->target.y) * smoothSpeed); 
}

static void DieAnimation(Scene *scene, const float dt){
  static float countdown = 2.0f;

  if(!scene->player->isAlive && !scene->isDead){ 
    scene->player->velocity.y = 0;
    scene->player->velocity.y -= 500.0f;
    
    if(scene->player->velocity.x < 0)
      scene->player->velocity.x += 800.0f;
    else
      scene->player->velocity.x -= 800.0f;
    
    scene->player->isGround = false;
    scene->player->life -= 1;
    scene->isDead = true; 
  }

  // update countdown
  if(scene->isDead){
    countdown -= dt;
    if(countdown <= 0){
      // reset player 
      scene->isDead = false;
      scene->player->isAlive = true;
      scene->player->velocity.x = 0;
      scene->player->velocity.y = 0;
      countdown = 2.0f;
      resetPlayerPosition(scene->player, &scene->maps->tileMaps[scene->currentMap]);
    }
  }
}

static void UpdateGameplay(Scene *scene){
  const float dt = GetFrameTime();
  
  UpdateMaps(&scene->maps->tileMaps[scene->currentMap], dt);
  if(scene->player->isAlive)
    UpdatePlayerCamera(scene->camera, &scene->player->position);
  EntityMove(scene->player, dt, &scene->maps->tileMaps[scene->currentMap], scene->camera);

  // TODO: FIX THIS
  DieAnimation(scene, dt); 
}

static void DrawGameplay(Scene *scene){
  DrawMaps(&scene->maps->tileMaps[scene->currentMap], scene->textures);
  EntityDraw(scene->player, scene->textures);
}

void UpdateScene(Scene *scene){
  switch(scene->sceneType){
    case SCENE_MAINMENU:
      break;
    case SCENE_GAMEPLAY:
      UpdateGameplay(scene);
      break;
    default:
      break;
  }
}

void DrawScene(Scene *scene){
  switch(scene->sceneType){
    case SCENE_MAINMENU:
      break;
    case SCENE_GAMEPLAY:{
      DrawGameplay(scene);
      break;
    }
    default:
      break;
  }
}

void resetPlayerPosition(Entity *player, TileMap *tileMap){
  player->position.x = tileMap->playerPosition.x;
  player->position.y = tileMap->playerPosition.y;
}
