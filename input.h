/* 
 * File:   input.h
 * Author: John
 *
 * Created on 2 février 2015, 19:53
 */

#ifndef INPUT_H
#define	INPUT_H
#include <array>
#include "SDL.h"
#include "utils.h"

//nb of SDL_Scancodes
static const int possible_keyboard_keys = 282;
static const int possible_mouse_buttons = 255;

enum eKey {
  kKey_Up,
  kKey_Down,
  kKey_Left,
  kKey_Right,
  kKey_And,
  kKey_Or,
  kKey_Inventory,
  kKey_Workshop,
  kKey_Copy,
  kKey_Paste,
  kKey_Select,
  kKey_Delete,
  kKey_Raise,
  kKey_Lower,
  kKey_ToggleHeight,
  kKey_None,
  kKey_Count
};

struct Input {
  Input();
  bool pollEvents();
	SDL_Event e;
  //link SDL_Scancode to eKey
  std::array<eKey, possible_keyboard_keys> keymap;
  
  std::array<bool, possible_mouse_buttons> mouse_buttons_down;
  std::array<bool, kKey_Count> keys_down;
  std::array<bool, kKey_Count> keys_up;
  Vecf mouse_position_in_window;
  Vecf mouse_position_in_world;
};

#endif	/* INPUT_H */

