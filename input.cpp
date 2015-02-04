#include <iostream>
#include "player.h"
#include "input.h"
#include "engine.h"
#include "gamedata.h"

Input::Input() : 
mouse_position_in_world{0.0f,0.0f}
,mouse_position_in_window{0,0}
,mouse_buttons_down{false}
,keys_down{false}
,keys_up{false}
,keymap{kKey_None},
delay_between_key_press(100),
rotate_left_key_press_timer(0){
  keymap[SDL_SCANCODE_S] = kKey_Down;
  keymap[SDL_SCANCODE_D] = kKey_Right;
  keymap[SDL_SCANCODE_A] = kKey_Left;
  keymap[SDL_SCANCODE_W] = kKey_Up;
  keymap[SDL_SCANCODE_1] = kKey_Wire;
  keymap[SDL_SCANCODE_2] = kKey_And;
  keymap[SDL_SCANCODE_3] = kKey_Or;
  keymap[SDL_SCANCODE_4] = kKey_Constant_1;
  keymap[SDL_SCANCODE_Q] = kKey_Rotate_Left;
  keymap[SDL_SCANCODE_E] = kKey_Rotate_Right;
}

bool Input::PollEvents(GameData& game_data, Engine& engine) {
  SDL_GetMouseState(&mouse_position_in_window.x, &mouse_position_in_window.y);
  mouse_position_in_world.x = mouse_position_in_window.x + engine.camera.x;
  mouse_position_in_world.y = mouse_position_in_window.y + engine.camera.y;
  
  if(rotate_left_key_press_timer < delay_between_key_press) {
    rotate_left_key_press_timer += g_delta_t;
  }
    
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT){
      return false;
    }
    
    if (e.type == SDL_KEYDOWN) {
      eKey key = keymap[e.key.keysym.scancode];
      
      keys_down[key] = true;
		}
      
    if (e.type == SDL_KEYUP) {
      keys_down[keymap[e.key.keysym.scancode]] = false;	
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {
      mouse_buttons_down[e.button.button] = true;
    }

    if (e.type == SDL_MOUSEBUTTONUP) {
      mouse_buttons_down[e.button.button] = false;
    }
  }
  
  if(keys_down[kKey_Rotate_Left]){
    if(rotate_left_key_press_timer < delay_between_key_press) {
      keys_down[kKey_Rotate_Left] = false;
    } else {
      rotate_left_key_press_timer = 0;
    }
  } 
  Vecf movement = game_data.ReceiveInput(keys_down, 
          mouse_buttons_down,
          mouse_position_in_window,
          mouse_position_in_world);
  engine.MoveCamera(movement);
  
  
  
  return true;
}


