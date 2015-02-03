/* 
 * File:   sprite_data.h
 * Author: John
 *
 * Created on 2 février 2015, 23:47
 */

#ifndef SPRITE_DATA_H
#define	SPRITE_DATA_H
#include <vector>
#include "SDL.h"


struct SpriteData {
  SpriteData();
  SpriteData(SDL_Texture* const texture_ptr, const int total_frame,
             std::vector<SDL_Rect> src_rect);
  SDL_Texture* texture_ptr;
  int total_frame;
  std::vector<SDL_Rect> src_rect;
};

#endif	/* SPRITE_DATA_H */

