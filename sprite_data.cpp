#include "sprite_data.h"

SpriteData::SpriteData() :
texture_ptr(nullptr)
,total_frame(0){

}

SpriteData::SpriteData(SDL_Texture * const texture_ptr, const int total_frame, 
                       std::vector<SDL_Rect> src_rect) :
texture_ptr(texture_ptr)
,total_frame(total_frame)
,src_rect(src_rect){

}

