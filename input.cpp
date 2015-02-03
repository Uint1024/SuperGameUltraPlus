#include <iostream>
#include "input.h"

Input::Input() : 
mouse_position_in_world{0.0f,0.0f}
,mouse_position_in_window{0.0f,0.0f}
,mouse_buttons_down{false}
,keys_down{false}
,keys_up{false}
,keymap{kKey_None}               
{
  /*for(auto &i : mouse_buttons_down){
    i = false;
  }
  for(auto &i : keys_down){
    i = false;
  }
  for(auto &i : keys_up){
    i = false;
  }
  for(auto &i : keymap){
    i = kKey_None;
  }
  */
  keymap[SDL_SCANCODE_S] = kKey_Down;
}

bool Input::pollEvents() {
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
  
  if (keys_down[kKey_Down]) {
    std::cout << "yay" << std::endl;
  }
  
  return true;
}


