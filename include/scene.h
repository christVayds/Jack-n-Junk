#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>
#include <raylib.h>
#include "entity.h"
#include "map.h"

typedef enum{
  SCENE_MAINMENU,
  SCENE_GAMEPLAY
} SceneType;

typedef struct{
  SceneType sceneType;
  Entity *player;
  Camera2D *camera;
  Maps *maps;
  Texture2D textures;
  uint32_t currentMap;
  bool isDead;
} Scene;

void UpdateScene(Scene *scene);
void DrawScene(Scene *scene);
void resetPlayerPosition(Entity *player, TileMap *tileMap);

#endif
