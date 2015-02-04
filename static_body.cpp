#include <iostream>

#include "SDL.h"
#include "engine.h"
#include "sprite_data.h"
#include "static_body.h"

StaticBody::StaticBody(const Vecf& position, const Veci& size, 
        const eTexture texture_id, const eDirection direction) :
SolidBody(position, size, texture_id, direction){
  
}

void StaticBody::Render(Engine& engine) {
  SpriteData* data = &engine.sprites_data[sprite.texture_id];
  
  SDL_Rect dest_rect = {
          static_cast<int>(bbox.left) - static_cast<int>(engine.camera.x), 
          static_cast<int>(bbox.top) - static_cast<int>(engine.camera.y), 
          bbox.size.x, 
          bbox.size.y};
  SDL_Rect* src_rect =  &data->src_rect[sprite.current_frame];
  int angle = 0;
  switch(direction){
    case kDirection_Left:
      angle = 90;
      break;
    case kDirection_Down:
      angle = 0;
      break;
    case kDirection_Right:
      angle = 270;
      break;
    case kDirection_Up:
      angle = 180;
      break;
  }
  SDL_RenderCopyEx(engine.renderer, data->texture_ptr, 
          src_rect,
          &dest_rect, angle, NULL, SDL_FLIP_NONE);
}


