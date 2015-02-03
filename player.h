/* 
 * File:   Player.h
 * Author: John
 *
 * Created on 2 février 2015, 20:30
 */

#ifndef PLAYER_H
#define	PLAYER_H
#include "solid.h"
#include "sprite_state.h"

struct Player {
  Player(const Vecf& pos);
  void render();
  Bbox bbox;
  SpriteState sprite_state;
};

#endif	/* PLAYER_H */

