/* 
 * File:   DynamicSprite.h
 * Author: John
 *
 * Created on 3 février 2015, 04:43
 */

#ifndef DYNAMICSPRITE_H
#define	DYNAMICSPRITE_H
#include "utils.h"

struct DynamicSprite {
  DynamicSprite(eTexture texture_id);
  
  int current_frame;
  eTexture texture_id;
  eDirection direction;
  int animation_timer;
  bool moving;
};

#endif	/* DYNAMICSPRITE_H */

