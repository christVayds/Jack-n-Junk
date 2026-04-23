#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>
#include <raylib.h>
#include "entity.h"
#include "map.h"

typedef enum{
  SCENE_OPENING,
  SCENE_INTRO,
  SCENE_MAINMENU,
  SCENE_GAMEPLAY,
  SCENE_CREDITS
} SceneType;

typedef struct{
  SceneType sceneType;
  SceneType targetScene;    // for transitioning scene 
  Entity *player;
  Camera2D *camera;
  Maps *maps;
  Texture2D textures;
  Font font;
  uint32_t currentMap;
  bool isDead;
  bool paused;
  Texture2D *backgrounds;     // list of backgrounds
  float transitionAlpha;    // transition alpha
  int32_t alphaDir;       // transition alpha direction
  bool doTrans;         // do transition 
} Scene;

void UpdateScene(Scene *scene);
void DrawScene(Scene *scene);
void DrawSceneBackground(Scene *scene);
void DrawSceneTransition(Scene *scene);
void InputScene(Scene *scene);
void resetPlayerPosition(Entity *player, TileMap *tileMap);

#endif
