#include "scene.h"
#include <math.h>
#include <stdio.h>

#define TEXTSPACING 1.0f
#define MAXTEXT 5
#define FONTSIZE 32.0f

int32_t currentText = 0;
char* texts[MAXTEXT] = {
  "Earth is abandoned by humanity a hundred years ago, \nleft to decay under pollution, \nendless waste, and a toxic atmosphere like Venus.",
  "Only machines remained, tasked with studying a dead world\nthat seemed beyond saving.",
  "One day, a group of B20W robots \ndiscovered something imposible, \na single sunflower, proof that life had returned.",
  "They rushed to the LIFE MACHINE to signal \nhumanity across distant space, \nbut hostile machines intervened and destroyed them all.",
  "Jack, the last surviving B20W unit, \nmust carry the flower to the LIFE MACHINE, \nracing against his limited battery and his own lack of combat to \nrestore hope for Earth."
};

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
  
  EntityUpdate(scene->player);
  EntityMove(scene->player, dt, &scene->maps->tileMaps[scene->currentMap], scene->camera);

  // TODO: FIX THIS
  DieAnimation(scene, dt); 
}

// DRAW GAMEPLAY 
static void DrawGameplay(Scene *scene){
  DrawMaps(&scene->maps->tileMaps[scene->currentMap], scene->textures, scene->font, 0);
  EntityDraw(scene->player, scene->textures);
  DrawMaps(&scene->maps->tileMaps[scene->currentMap], scene->textures, scene->font, 1);
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
    DrawTextureRec(scene->textures, YANJILOGO, (Vector2){scene->camera->target.x - YANJILOGO.width/2, scene->camera->target.y - (YANJILOGO.height/2) - 200}, Fade(WHITE, fadeAlpha)); 
  } else
    DrawTextEx(scene->font, text, position, FONTSIZE, TEXTSPACING, Fade(WHITE, fadeAlpha));
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
          currentText = 0;
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
      break;
    case SCENE_INTRO:
      if(IsKeyPressed(KEY_SPACE)){
        currentText++;
        
        if(currentText >= MAXTEXT){
          scene->doTrans = true;
          scene->targetScene = SCENE_GAMEPLAY;
          currentText = MAXTEXT-1;
        };
      }
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
    case SCENE_GAMEPLAY:{ 
      DrawTextureEx(scene->backgrounds[0], (Vector2){offsetX, offsetY}, 0.0f, scale, WHITE);    
      break;
    }
    case SCENE_INTRO:{
      DrawTextureEx(scene->backgrounds[0], (Vector2){offsetX, offsetY}, 0.0f, scale, WHITE);
    }
    default:
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
