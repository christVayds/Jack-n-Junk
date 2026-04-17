#include "scene.h"
#include <math.h>

static void UpdatePlayerCamera(Camera2D *camera, Vector2 *playerposition){
  float smoothSpeed = 0.4f;
  camera->target.x += floorf((playerposition->x - camera->target.x) * smoothSpeed);
  camera->target.y += floorf((playerposition->y - camera->target.y) * smoothSpeed); 
}

static void UpdateGameplay(Scene *scene){
  const float dt = GetFrameTime();
  UpdatePlayerCamera(scene->camera, &scene->player->position);
  EntityMove(scene->player, dt, &scene->maps->tileMaps[scene->currentMap], scene->camera);
}

static void DrawGameplay(Scene *scene){
  DrawMaps(&scene->maps->tileMaps[scene->currentMap]);
  EntityDraw(scene->player);
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
