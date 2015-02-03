#include <iostream>
#include "player.h"
#include "input.h"

Input::Input() : 
mouse_position_in_world{0.0f,0.0f}
,mouse_position_in_window{0.0f,0.0f}
,mouse_buttons_down{false}
,keys_down{false}
,keys_up{false}
,keymap{kKey_None} {
  keymap[SDL_SCANCODE_S] = kKey_Down;
  keymap[SDL_SCANCODE_D] = kKey_Right;
  keymap[SDL_SCANCODE_A] = kKey_Left;
  keymap[SDL_SCANCODE_W] = kKey_Up;
}

bool Input::PollEvents(Player& player) {
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
  
  player.ReceiveInput(keys_down, mouse_buttons_down);
  
  return true;
}


