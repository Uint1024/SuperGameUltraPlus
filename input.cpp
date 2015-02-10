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
,last_keys_down{false}
,keymap{kKey_None},
delay_between_key_press(100),
rotate_key_press_timer(0){
  keymap[SDL_SCANCODE_S] = kKey_Down;
  keymap[SDL_SCANCODE_D] = kKey_Right;
  keymap[SDL_SCANCODE_A] = kKey_Left;
  keymap[SDL_SCANCODE_W] = kKey_Up;
  keymap[SDL_SCANCODE_1] = kKey_ToolBar1;
  keymap[SDL_SCANCODE_2] = kKey_ToolBar2;
  keymap[SDL_SCANCODE_3] = kKey_ToolBar3;
  keymap[SDL_SCANCODE_4] = kKey_ToolBar4;
  keymap[SDL_SCANCODE_5] = kKey_ToolBar5;
  keymap[SDL_SCANCODE_6] = kKey_ToolBar6;
  keymap[SDL_SCANCODE_7] = kKey_ToolBar7;
  keymap[SDL_SCANCODE_9] = kKey_ToolBar9;
  keymap[SDL_SCANCODE_EQUALS] = kKey_CreateLabel;
  keymap[SDL_SCANCODE_Q] = kKey_Rotate_Left;
  keymap[SDL_SCANCODE_E] = kKey_Rotate_Right;
  keymap[SDL_SCANCODE_LSHIFT] = kKey_Select_To_Save;
  keymap[SDL_SCANCODE_KP_MINUS] = kKey_Speed_Down;
  keymap[SDL_SCANCODE_KP_PLUS] = kKey_Speed_Up;
  keymap[SDL_SCANCODE_RETURN] = kKey_Enter;
  //keymap[SDL_SCANCODE_TAB] = kKey_Load;
  keymap[SDL_SCANCODE_C] = kKey_Copy;
  keymap[SDL_SCANCODE_V] = kKey_Paste;
  keymap[SDL_SCANCODE_DELETE] = kKey_Delete;
  keymap[SDL_SCANCODE_B] = kKey_Brush;
  keymap[SDL_SCANCODE_F] = kKey_Fill;
  keymap[SDL_SCANCODE_PAGEDOWN] = kKey_Zoom_Out;
  keymap[SDL_SCANCODE_PAGEUP] = kKey_Zoom_In;
  keymap[SDL_SCANCODE_BACKSPACE] = kKey_Backspace;
  keymap[SDL_SCANCODE_F5] = kKey_QuickSave;
  keymap[SDL_SCANCODE_F9] = kKey_QuickLoad;
  keymap[SDL_SCANCODE_R] = kKey_Toggle;
}

bool Input::PollEvents(GameData& game_data, Engine& engine) {
  SDL_GetMouseState(&mouse_position_in_window.x, &mouse_position_in_window.y);
  mouse_position_in_world.x = mouse_position_in_window.x + engine.camera.x;
  mouse_position_in_world.y = mouse_position_in_window.y + engine.camera.y;
  
  mouse_buttons_down[kKey_Mouse_Rotate_Right] = false;
  mouse_buttons_down[kKey_Mouse_Rotate_Left] = false;
          
  if(rotate_key_press_timer < delay_between_key_press) {
    rotate_key_press_timer += g_delta_t;
  }
  
  if(game_data.create_label_mode){
    SDL_StartTextInput();
  }
  else{
    SDL_StopTextInput();
  }
  
  std::string text_input;
  while (SDL_PollEvent(&e) != 0) {
    eKey key = kKey_None;
    //key = keymap[e.key.keysym.scancode];
    switch(e.type){
      case SDL_QUIT:
        return false;
        break;
      case SDL_KEYDOWN:
        keys_down[keymap[e.key.keysym.scancode]] = true;
        break;
      case SDL_KEYUP:
        keys_down[keymap[e.key.keysym.scancode]] = false;	
        break;

      case SDL_MOUSEBUTTONDOWN:
        mouse_buttons_down[e.button.button] = true;
        break;

      case SDL_MOUSEBUTTONUP:
        mouse_buttons_down[e.button.button] = false;
        break;
        
      case SDL_MOUSEWHEEL:
        if(e.wheel.y > 0){
          mouse_buttons_down[kKey_Mouse_Rotate_Left] = true;
        }
        else if(e.wheel.y < 0){
          mouse_buttons_down[kKey_Mouse_Rotate_Right] = true;
        }        
        break;
      case SDL_TEXTINPUT:
        text_input.append(e.text.text);
        break;
    }
    
    if(game_data.save_gate_window) {
      if(e.type == SDL_TEXTINPUT){
        strcat(game_data.save_gate_window->gate_name, e.text.text);
      }
    }    
  }
  
 
  Vecf movement = game_data.ReceiveInput(text_input,
          keys_down, 
          mouse_buttons_down,
          mouse_position_in_window,
          mouse_position_in_world,
          last_keys_down,
          last_mouse_buttons_down);
  engine.MoveCamera(movement);
  
  last_keys_down = keys_down;
  last_mouse_buttons_down = mouse_buttons_down;
  
  return true;
}


