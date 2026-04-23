#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <raylib.h>

#include "game.h"
#include "scene.h"
#include "entity.h"
#include "map.h"

//#include <emscripten/emscripten.h>

static void UpdateGame(void);
Scene scene = {0};
Camera2D camera = {0};
Texture2D background;

int main(){
  const int32_t monitor = GetCurrentMonitor();
  const int32_t monitorWidth = GetMonitorWidth(monitor);
  const int32_t monitorHeight = GetMonitorHeight(monitor);
  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(monitorWidth, monitorHeight, "Jack 'n Junk");
  SetTargetFPS(MAXFPS);

  // MAPS 
  uint32_t Map[MAPCOUNT][MAPWIDTH*MAPHEIGHT] = {
    {
      11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      11,0,0,0,0,0,39,0,37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      11,0,36,0,0,0,2,1,1,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      11,0,0,0,0,0,6,7,7,7,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      11,10,0,0,0,0,35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      25,22,23,0,26,0,0,28,27,0,30,27,31,29,32,0,0,34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,12,
      4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5,0,
      6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,0
    }
  };

  // initilize map
  Maps maps = {0};
  if(!InitMaps(&maps, Map)) return 1;

  // Set Background 
  background = LoadTexture("resources/textures/background.png");

  // CAMERA 
  camera.target         = (Vector2){0,0};
  camera.offset         = (Vector2){GetScreenWidth()/2.0f, (GetScreenHeight()/2.0f) + 128};
  camera.rotation       = 0.0f;
  camera.zoom           = 1.0f;

  // Initialize PLAYER 
  Entity player = EntityNew((Vector2){250, 250});

  // Initialize scene 
  scene.sceneType = SCENE_OPENING;
  scene.player = &player;
  scene.isDead = false;
  scene.camera = &camera;
  scene.maps = &maps;
  scene.currentMap = 0;
  scene.textures = LoadTexture("resources/textures/textures.png");
  scene.font = LoadFont("resources/fonts/Tiny5-Regular.ttf");
  scene.paused = true;
  scene.transitionAlpha = 0.0f;
  scene.doTrans = false;
  scene.alphaDir = 1;

  scene.backgrounds = (Texture2D*)malloc(sizeof(Texture2D));
  if(!scene.backgrounds) return 1;
  scene.backgrounds[0] = background;

  // NOTE: temporary set player position
  resetPlayerPosition(&player, &maps.tileMaps[scene.currentMap]);
  
  // EXE 
  while(!WindowShouldClose()){
    UpdateGame();
  }

  // BROWSER
  //emscripten_set_main_loop(UpdateGame, 0, 1);

  FreeMaps(&maps);
  UnloadTexture(scene.textures);
  UnloadFont(scene.font);
  UnloadTexture(background);
  free(scene.backgrounds);
  CloseWindow();
  return 0;
}

static void UpdateGame(void){
  // UPDATE 
  UpdateScene(&scene);

  // INPUT 
  InputScene(&scene); 

  // DRAWING
  BeginDrawing();
    // clear backgound
    ClearBackground((Color){197, 161, 0, 255});

    // draw scene background
    DrawSceneBackground(&scene);
    
    // Camera mode
    BeginMode2D(camera);
      // DRAW
      //DrawTextureRec(scene.textures, (Rectangle){400, 128, PLAYER_WIDTH, PLAYER_HEIGHT}, (Vector2){100, 0}, WHITE);
      DrawScene(&scene);
    EndMode2D();

    // DRAW SCENE TRANSITION
    DrawSceneTransition(&scene);


    // DEBUGGING
    if(DEBUG_MODE){
      DrawFPS(GetScreenWidth() - 100, 10);
    }
  EndDrawing();
}
