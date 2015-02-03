/* 
 * File:   Player.h
 * Author: John
 *
 * Created on 2 février 2015, 20:30
 */

#ifndef PLAYER_H
#define	PLAYER_H
#include <array>
#include "solid.h"
#include "utils.h"
#include "sprite_state.h"
class Engine;

struct Player {
  Player(const Vecf& pos);
  void Render(Engine* engine);
  void ReceiveInput(std::array<bool, kKey_Count> keys_down,
                    std::array<bool, 255> mouse_buttons_down);
  Bbox bbox;
  SpriteState sprite_state;
};

#endif	/* PLAYER_H */

