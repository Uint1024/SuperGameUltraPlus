#include "SDL.h"
#include "engine.h"
#include "sprite_data.h"
#include "dynamic_body.h"

DynamicBody::DynamicBody(const Vecf& position, const Veci& size, 
        const eTexture texture_id, const eDirection direction) :
SolidBody(position, size, texture_id, direction){

}

void DynamicBody::Render(Engine& engine) {
  SpriteData* data = &engine.sprites_data[sprite.texture_id];
  
  SDL_Rect dest_rect = {
          static_cast<int>(bbox.left) - static_cast<int>(engine.camera.x), 
          static_cast<int>(bbox.top) - static_cast<int>(engine.camera.y), 
          bbox.size.x, 
          bbox.size.y};
  SDL_Rect* src_rect =  &data->src_rect[sprite.current_frame];
  SDL_RenderCopyEx(engine.renderer, data->texture_ptr, 
          src_rect,
          &dest_rect, 0, NULL, SDL_FLIP_NONE);
}



