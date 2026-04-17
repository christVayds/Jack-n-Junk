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

int main(){
  InitWindow(SCREENWIDTH, SCREENHEIGHT, "Jack 'n Junk");
  SetTargetFPS(MAXFPS);

  // MAPS 
  uint32_t Map[MAPCOUNT][MAPWIDTH*MAPHEIGHT] = {
    {
      0,10,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,
      1,1,1,1,1,1,1,1,1,1
    }
  };

  // initilize map
  Maps maps = {0};
  if(!InitMaps(&maps, Map)) return 1;

  // CAMERA 
  camera.target         = (Vector2){0,0};
  camera.offset         = (Vector2){SCREENWIDTH/2.0f, SCREENHEIGHT/2.0f};
  camera.rotation       = 0.0f;
  camera.zoom           = 1.0f;

  // Initialize PLAYER 
  Entity player = EntityNew((Vector2){250, 250}, (Rectangle){0, 0, PLAYER_WIDTH, PLAYER_HEIGHT}, 100);

  // Initialize scene 
  scene.sceneType = SCENE_GAMEPLAY;
  scene.player = &player;
  scene.camera = &camera;
  scene.maps = &maps;
  scene.currentMap = 0;

  // NOTE: temporary set player position
  resetPlayerPosition(&player, &maps.tileMaps[scene.currentMap]);
  
  // EXE 
  while(!WindowShouldClose()){
    UpdateGame();
  }

  // BROWSER
  //emscripten_set_main_loop(UpdateGame, 0, 1);

  FreeMaps(&maps);
  CloseWindow();
  return 0;
}

static void UpdateGame(void){
  // UPDATE 
  UpdateScene(&scene);

  // DRAWING
  BeginDrawing();
    // clear backgound
    ClearBackground(BLACK);
    
    // Camera mode
    BeginMode2D(camera);
      // DRAW
      DrawScene(&scene);
    EndMode2D();

    // DEBUGGING
    if(DEBUG_MODE)
      DrawFPS(SCREENWIDTH - 100, 10);
  EndDrawing();
}
