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
class Player;
class GameData;

static const int possible_keyboard_keys = 282;
struct Input {
  Input();
  bool PollEvents(GameData& game_data, Engine& engine);
	SDL_Event e;
  //link SDL_Scancode to eKey
  std::array<eKey, possible_keyboard_keys> keymap;
  
  std::array<bool, 255> mouse_buttons_down;
  std::array<bool, kKey_Count> keys_down;
  std::array<bool, kKey_Count> last_keys_down;
  std::array<bool, kKey_Count> keys_up;
  Veci mouse_position_in_window;
  Vecf mouse_position_in_world;
          
            
  int rotate_key_press_timer;
  int delay_between_key_press;
};

#endif	/* INPUT_H */

