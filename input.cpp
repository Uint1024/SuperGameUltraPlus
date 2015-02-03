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
,keymap{kKey_None}{
  keymap[SDL_SCANCODE_S] = kKey_Down;
  keymap[SDL_SCANCODE_D] = kKey_Right;
  keymap[SDL_SCANCODE_A] = kKey_Left;
  keymap[SDL_SCANCODE_W] = kKey_Up;
  keymap[SDL_SCANCODE_1] = kKey_Wire;
  keymap[SDL_SCANCODE_2] = kKey_And;
  keymap[SDL_SCANCODE_3] = kKey_Or;
  keymap[SDL_SCANCODE_4] = kKey_Constant_1;
}

bool Input::PollEvents(GameData& game_data, Engine& engine) {
  SDL_GetMouseState(&mouse_position_in_window.x, &mouse_position_in_window.y);
  mouse_position_in_world.x = mouse_position_in_window.x + engine.camera.x;
  mouse_position_in_world.y = mouse_position_in_window.y + engine.camera.y;
  
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT){
      return false;
    }
    
    if (e.type == SDL_KEYDOWN) {
      keys_down[keymap[e.key.keysym.scancode]] = true;
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
  
  Vecf movement = game_data.ReceiveInput(keys_down, 
          mouse_buttons_down,
          mouse_position_in_window,
          mouse_position_in_world);
  engine.MoveCamera(movement);
  
  
  
  return true;
}


