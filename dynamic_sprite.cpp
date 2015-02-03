
#include <iostream>
#include "dynamic_sprite.h"
#include "sprite_data.h"
#include "utils.h"

DynamicSprite::DynamicSprite(eTexture texture_id) :
texture_id(texture_id)
,current_frame(0)
,direction(kDirection_Down)
,animation_timer(0)
,moving(false)
{

}
