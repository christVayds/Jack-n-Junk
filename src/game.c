#include "game.h"

// return the x1, xN, y1, yN blocks that visible to player's camera 
WorldPoints BlocksVisibleInCamera(Camera2D *camera){
  float camLeft = camera->target.x - (SCREENWIDTH / 2.0f) / camera->zoom;
  float camTop  = camera->target.y - (SCREENHEIGHT / 2.0f) / camera->zoom;
  float camRight  = camera->target.x + (SCREENWIDTH / 2.0f) / camera->zoom;
  float camBottom = camera->target.y + (SCREENHEIGHT / 2.0f) / camera->zoom;
  
  int32_t startX = (int)(camLeft / TILESIZE) - 1;
  int32_t startY = (int)(camTop / TILESIZE) - 1;
  int32_t endX = (int)(camRight / TILESIZE) + 1;
  int32_t endY = (int)(camBottom / TILESIZE) + 1;

  // clamp
  if(startX < 0) startX = 0;
  if(startY < 0) startY = 0;
  if(endX > MAPWIDTH) endX = MAPWIDTH;
  if(endY > MAPHEIGHT) endY = MAPHEIGHT;

  return (WorldPoints){startX, startY, endX, endY};
}
