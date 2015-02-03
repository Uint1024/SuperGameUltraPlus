/* 
 * File:   Sprite.h
 * Author: John
 *
 * Created on 2 février 2015, 23:17
 */

#ifndef SPRITE_H
#define	SPRITE_H
#include <vector>
#include "SDL.h"
#include "utils.h"
class Bbox;

struct SpriteState {
  SpriteState(eTexture texture_id);
  
  int current_frame;
  eTexture texture_id;
  eDirection direction;
  Bbox* body;
  int animation_timer;
  bool moving;
};

#endif	/* SPRITE_H */

