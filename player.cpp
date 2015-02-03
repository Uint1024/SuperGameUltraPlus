#include <iostream>
#include "sprite_data.h"
#include "globals.h"
#include "player.h"

Player::Player(const Vecf& pos) :
bbox(pos, Veci{128, 128}),
sprite_state(kTexture_Player){
}

void
Player::render() {
  SpriteData* data = &g_engine.sprites_data[sprite_state.texture_id];
  
  SDL_Rect dest_rect = {
          static_cast<int>(bbox.left), 
          static_cast<int>(bbox.top), 
          bbox.size.x, 
          bbox.size.y};
  SDL_RenderCopy(g_engine.renderer, 
          data->texture_ptr, 
          &data->src_rect[sprite_state.current_frame],
          &dest_rect);
}


