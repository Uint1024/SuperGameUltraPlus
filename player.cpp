#include <iostream>
#include "sprite_data.h"
#include "player.h"
#include "engine.h"

Player::Player(const Vecf& pos) :
bbox(pos, Veci{128, 128}),
sprite_state(kTexture_Player){
}

void
Player::Render(Engine* engine) {
  SpriteData* data = &engine->sprites_data[sprite_state.texture_id];
  
  SDL_Rect dest_rect = {
          static_cast<int>(bbox.left), 
          static_cast<int>(bbox.top), 
          bbox.size.x, 
          bbox.size.y};
  SDL_RenderCopy(engine->renderer, 
          data->texture_ptr, 
          &data->src_rect[sprite_state.current_frame],
          &dest_rect);
}

void Player::ReceiveInput(std::array<bool, kKey_Count> keys_down,
                    std::array<bool, 255> mouse_buttons_down) {
  Vecf movement = {0.0f,0.0f};
  if(keys_down[kKey_Right]){
    movement.x += 0.5 * g_delta_t;
  }
  if(keys_down[kKey_Left]){
    movement.x -= 0.5 * g_delta_t;
  }
  
  bbox.Move(movement);
}
