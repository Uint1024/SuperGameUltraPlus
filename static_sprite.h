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

struct StaticSprite {
  StaticSprite(const eTexture texture_id);
  eTexture texture_id;
};

#endif	/* SPRITE_H */

