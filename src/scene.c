#include "scene.h"
#include <math.h>
#include <stdio.h>

#define TEXTSPACING 1.0f
#define MAXTEXT 5
#define MAXENDINGTEXT 2
#define FONTSIZE 32.0f

int32_t currentText = 0;
int32_t prevLevel = 0;
bool playerWin = false;

char* texts[MAXTEXT] = {
  "Earth is abandoned by humanity a hundred years ago, \nleft to decay under pollution, \nendless waste, and a toxic atmosphere like Venus.",
  "Only machines remained, tasked with studying a dead world\nthat seemed beyond saving.",
  "One day, a group of B20W robots \ndiscovered something imposible, \na single sunflower, proof that life had returned.",
  "They rushed to the LIFE MACHINE to signal \nhumanity across distant space, \nbut hostile machines intervened and destroyed them all.",
  "Jack, the last surviving B20W unit, \nmust carry the flower to the LIFE MACHINE, \nracing against his limited battery and his own lack of combat to \nrestore hope for Earth."
};

int32_t currentEndingText = 0;
char* endingWin[MAXENDINGTEXT] = {
  "You successfully send the signal to humans",
  "the end"
};

char* endingLoss[MAXENDINGTEXT] = {
  "Your battery died",
  "The end"
};

static void UpdatePlayerCamera(Camera2D *camera, Vector2 *playerposition){
  float smoothSpeed = 0.4f;
  camera->target.x += floorf((playerposition->x - camera->target.x) * smoothSpeed);
  camera->target.y += floorf((playerposition->y - camera->target.y) * smoothSpeed); 
}

// NOTE: WHY THIS FUNCTION IS IN HERE?
static void DieAnimation(Scene *scene, const float dt){
  static float countdown = 2.0f;

  if(!scene->player->isAlive && !scene->isDead){
    PlaySound(scene->sounds[1]);
    scene->player->life -= 1;
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

  // if player fall in void
  if(scene->player->position.y > scene->maps->tileMaps[scene->currentMap].tiles[MAPWIDTH*MAPHEIGHT - 1].position.y)
    scene->player->isAlive = false;
}

static void UpdateGameplay(Scene *scene){
  const float dt = GetFrameTime();

  UpdateMaps(&scene->maps->tileMaps[scene->currentMap], dt);
  
  // check if player is alive
  if(scene->player->isAlive)
    UpdatePlayerCamera(scene->camera, &scene->player->position);

  // check entity level 
  if(scene->player->level > prevLevel){
    if(scene->player->level >= MAPCOUNT){
      playerWin = true;
      scene->doTrans = true;
      scene->targetScene = SCENE_ENDGAME;
    } else {
      scene->doTrans = true;
      scene->targetScene = SCENE_GAMEPLAY;
      prevLevel = scene->player->level;
    }
  }

  // update entity battery level
  if(scene->player->battery <= 0.0f){
    scene->doTrans = true;
    scene->targetScene = SCENE_ENDGAME;
    playerWin = false;
  }
   
  EntityMove(scene->player, dt, &scene->maps->tileMaps[scene->currentMap], scene->camera, scene->sounds);
  EntityUpdate(scene->player);

  // TODO: FIX THIS
  DieAnimation(scene, dt); 
}

// DRAW GAMEPLAY 
static void DrawGameplay(Scene *scene){
  WorldPoints points = BlocksVisibleInCamera(scene->camera);
  DrawMaps(&scene->maps->tileMaps[scene->currentMap], scene->textures, scene->font, 0, &points);
  EntityDraw(scene->player, scene->textures);
  DrawMaps(&scene->maps->tileMaps[scene->currentMap], scene->textures, scene->font, 1, &points);
}

// OPENING SCENE 
static void DrawOpeningScene(Scene *scene){
  const char* text = "An Entry for Gamedev.js Game Jam 2026";
  static float fadeAlpha = 0.0f;
  static int32_t fadeDir = 1;
  static int32_t show = 0;
  const int32_t limit = 1;

  fadeAlpha += 0.01f * fadeDir;
  if(fadeDir && fadeAlpha >= 1.0f){
    fadeDir = -1;
    
    if(show == limit){
      scene->doTrans = true;
      scene->targetScene = SCENE_INTRO; 
      fadeDir = 1;
      fadeAlpha = 1.0f;
    }
  } else if(fadeDir < 0 && fadeAlpha <= 0.0f){
    fadeAlpha = 0.0f;
    fadeDir = 1; 
    show++;
  }
  
  Vector2 textSize = MeasureTextEx(scene->font, text, FONTSIZE, TEXTSPACING);
  Vector2 position = (Vector2){
    scene->camera->target.x - textSize.x/2,
    scene->camera->target.y - FONTSIZE/2
  };
 
  if(show){
    DrawTextureRec(scene->textures, YANJILOGO, (Vector2){scene->camera->target.x - YANJILOGO.width/2, scene->camera->target.y - YANJILOGO.height/2 - 100}, Fade(WHITE, fadeAlpha)); 
  } else
    DrawTextEx(scene->font, text, position, FONTSIZE, TEXTSPACING, Fade(WHITE, fadeAlpha));
}

// DRAW ENDGAME
static void DrawEndGame(Scene *scene){
  char** endingText = endingWin;
  if(!playerWin) endingText = endingLoss;

  Vector2 textSize = MeasureTextEx(scene->font, endingText[currentEndingText], FONTSIZE, TEXTSPACING);
  Vector2 position = (Vector2){
    scene->camera->target.x - textSize.x/2, 
    scene->camera->target.y - FONTSIZE/2
  };

  DrawTextEx(scene->font, endingText[currentEndingText], position, FONTSIZE, TEXTSPACING, WHITE);
  DrawTextEx(scene->font, "Press SPACE", (Vector2){scene->camera->target.x, scene->camera->target.y + 200.0f}, FONTSIZE, TEXTSPACING, WHITE);
}

void UpdateScene(Scene *scene){
  
  // update transition 
  if(scene->doTrans){
    scene->transitionAlpha += 0.01f * scene->alphaDir;

    if(scene->alphaDir && scene->transitionAlpha >= 1.0f){
      switch(scene->targetScene){
        case SCENE_GAMEPLAY:{
          scene->sceneType = SCENE_GAMEPLAY;
          scene->camera->offset = (Vector2){GetScreenWidth()/2.0f, (GetScreenHeight()/2.0f) + 128};
          scene->camera->zoom = 2.0f;
          scene->currentMap = scene->player->level;
          resetPlayerPosition(scene->player, &scene->maps->tileMaps[scene->currentMap]);
          break;
        }
        case SCENE_MAINMENU:{
          scene->camera->zoom = 2.0f;
          scene->sceneType = SCENE_MAINMENU;
          resetPlayerPosition(scene->player, &scene->maps->tileMaps[scene->currentMap]);
          break;
        }
        case SCENE_INTRO:{
          scene->sceneType = SCENE_INTRO;
          PlayMusicStream(scene->music);
          break;
        }
        case SCENE_ENDGAME:{
          scene->sceneType = SCENE_ENDGAME;
          scene->camera->zoom = 1.0f;
          scene->currentMap = 0;
          scene->player->level = 0;
          resetPlayerPosition(scene->player, &scene->maps->tileMaps[scene->currentMap]);
          ResetMap(scene->maps);
          break;
        }
        default:
          scene->sceneType = scene->targetScene; 
          break;
      } 
      scene->alphaDir = -1;
    }
    if(scene->alphaDir < 0 && scene->transitionAlpha <= 0.0f){
      scene->alphaDir = 1;
      scene->transitionAlpha = 0;
      scene->doTrans = false;
    }
  }

  switch(scene->sceneType){
    case SCENE_MAINMENU:
      UpdateGameplay(scene);
      break;
    case SCENE_GAMEPLAY:
      UpdateGameplay(scene);
      break;
    case SCENE_INTRO:
      break;
    default:
      break;
  }
}

void InputScene(Scene *scene){
  switch(scene->sceneType){
    case SCENE_GAMEPLAY:
      break;
    case SCENE_MAINMENU:
      if(IsKeyPressed(KEY_D) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_SPACE)) {
        //scene->doTrans = true;
        //scene->targetScene = SCENE_GAMEPLAY;
        scene->sceneType = SCENE_GAMEPLAY;
        scene->camera->offset = (Vector2){GetScreenWidth()/2.0f, (GetScreenHeight()/2.0f) + 128};
      }
      break;
    case SCENE_INTRO:
      if(IsKeyPressed(KEY_SPACE)){
        currentText++;
        
        if(currentText >= MAXTEXT){
          scene->doTrans = true;
          scene->targetScene = SCENE_MAINMENU;
          currentText = MAXTEXT-1;
        };
      }
      break;
    case SCENE_ENDGAME:{
      if(IsKeyPressed(KEY_SPACE)){
        currentEndingText++;

        if(currentEndingText >= MAXENDINGTEXT){
          scene->doTrans = true;
          scene->targetScene = SCENE_MAINMENU;
          currentEndingText = MAXENDINGTEXT - 1;
        }
      }
    }
    default:
      break;
  }
}

void DrawScene(Scene *scene){
  switch(scene->sceneType){
    case SCENE_MAINMENU:
      // TODO: THIS IS TEMPORARY
      DrawGameplay(scene);
      
      Vector2 position = (Vector2){
        scene->camera->target.x - 434/2,
        scene->camera->target.y - 200
      };
      
      DrawTextureRec(scene->textures, (Rectangle){0, 608, 434, 53}, position, WHITE);

      Vector2 textSize = MeasureTextEx(scene->font, "Move to start game", 15.0f, TEXTSPACING);
      DrawTextEx(scene->font, "Move to start", (Vector2){scene->camera->target.x-textSize.x/2, (scene->camera->target.y-15.0f)+100.0f}, 15.0f, TEXTSPACING, WHITE);
      break;
    
    case SCENE_GAMEPLAY:{
      DrawGameplay(scene);
      break;
    }
    case SCENE_INTRO:{
      Vector2 textSize = MeasureTextEx(scene->font, texts[currentText], FONTSIZE, TEXTSPACING);
      Vector2 position = (Vector2){
        scene->camera->target.x - textSize.x/2, 
        scene->camera->target.y - FONTSIZE/2
      };

      DrawTextEx(scene->font, texts[currentText], position, FONTSIZE, TEXTSPACING, WHITE);
      DrawTextEx(scene->font, "Press SPACE", (Vector2){scene->camera->target.x, scene->camera->target.y + 200.0f}, FONTSIZE, TEXTSPACING, WHITE);
      break;
    }
    case SCENE_OPENING:{
      DrawOpeningScene(scene); 
      break;
    }
    case SCENE_ENDGAME:{
      DrawEndGame(scene);
      break;
    }
    default:
      break;
  }
}

void DrawSceneBackground(Scene *scene){
  float scaleX = (float)GetScreenWidth() / BACKGROUNDWIDTH;
  float scaleY = (float)GetScreenHeight() / BACKGROUNDHEIGHT;
  float scale = (scaleX < scaleY) ? scaleX : scaleY;

  float newWidth = BACKGROUNDWIDTH * scale;
  float newHeight = BACKGROUNDHEIGHT * scale;

  float offsetX = (GetScreenWidth() - newWidth) / 2;
  float offsetY = (GetScreenHeight() - newHeight) / 2;
  
  switch(scene->sceneType){
    case SCENE_OPENING:{
      ClearBackground((Color){27, 38, 50, 255});
      break;
    } 
    default:
      DrawTextureEx(scene->backgrounds[scene->currentBg], (Vector2){offsetX, offsetY}, 0.0f, scale, WHITE);
      break;
  }
}

void DrawSceneTransition(Scene *scene){
  DrawRectangleRec((Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, Fade(BLACK, scene->transitionAlpha));
}

void resetPlayerPosition(Entity *player, TileMap *tileMap){
  player->position.x = tileMap->playerPosition.x;
  player->position.y = tileMap->playerPosition.y;
  if(player->textureRec.width < 0)
    player->textureRec.width *= -1;
}
