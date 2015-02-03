/* 
 * File:   Player.h
 * Author: John
 *
 * Created on 2 février 2015, 20:30
 */

#ifndef PLAYER_H
#define	PLAYER_H
#include <array>
#include "utils.h"
#include "solid_body.h"

class Engine;

struct Player {
  Player(const Vecf& pos);
  Vecf ReceiveInput(std::array<bool, kKey_Count> keys_down,
                    std::array<bool, 255> mouse_buttons_down);
  SolidBody body;

};

#endif	/* PLAYER_H */

