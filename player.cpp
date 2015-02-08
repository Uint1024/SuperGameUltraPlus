#include <iostream>
#include <cmath>
#include "sprite_data.h"
#include "player.h"
#include "engine.h"
#include "dynamic_body.h"

Player::Player(const Vecf& pos) :
body(new DynamicBody(pos, Veci{64, 64}, kTexture_Player, kDirection_Down)){
}

Vecf Player::ReceiveInput(std::array<bool, kKey_Count> keys_down,
                    std::array<bool, 255> mouse_buttons_down) {
  Vecf movement = {0.0f,0.0f};
  float speed = 1.2f;
  if(keys_down[kKey_Right]){
    if(keys_down[kKey_Up] || keys_down[kKey_Down]){
      speed = std::sqrt((speed * speed)/2);
    }
  }
  if(keys_down[kKey_Left]){
    if(keys_down[kKey_Up] || keys_down[kKey_Down]){
      speed = std::sqrt((speed * speed)/2);
    }
  }
  if(keys_down[kKey_Right]){
    movement.x += speed * g_delta_t;
  }
  if(keys_down[kKey_Left]){
    movement.x -= speed * g_delta_t;
  }
  if(keys_down[kKey_Up]){
    movement.y -= speed * g_delta_t;
  }
  if(keys_down[kKey_Down]){
    movement.y += speed * g_delta_t;
  }
  
  if(movement.x != 0.0f || movement.y != 0.0f){
    body->sprite.moving = true;
    body->sprite.animation_timer += g_delta_t;
    if(body->sprite.animation_timer > 150){
      ++body->sprite.current_frame;
      if(body->sprite.current_frame == 3){
        body->sprite.current_frame = 0;
      }
      body->sprite.animation_timer  = 0;
    }
  }
  body->bbox.Move(movement);
  
  return movement;
}
