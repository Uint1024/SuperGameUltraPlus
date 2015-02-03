#include "solid_body.h"
#include "engine.h"

SolidBody::SolidBody(const Vecf& position, const Veci& size, 
        const eTexture texture_id) :
bbox(position, size),
sprite(texture_id){
  
}


void SolidBody::Render(Engine& engine) {
  SpriteData* data = &engine.sprites_data[sprite.texture_id];
  
  SDL_Rect dest_rect = {
          static_cast<int>(bbox.left) - static_cast<int>(engine.camera.x), 
          static_cast<int>(bbox.top) - static_cast<int>(engine.camera.y), 
          bbox.size.x, 
          bbox.size.y};
  SDL_RenderCopy(engine.renderer, 
          data->texture_ptr, 
          &data->src_rect[sprite.current_frame],
          &dest_rect);
}

