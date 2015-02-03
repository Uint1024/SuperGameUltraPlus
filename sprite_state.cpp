#include <iostream>
#include "sprite_state.h"
#include "sprite_data.h"
#include "utils.h"
#include "solid.h"

SpriteState::SpriteState(eTexture texture_id) :
texture_id(texture_id)
,current_frame(0)
,direction(kDirection_Down)
,body(nullptr)
,animation_timer(0)
,moving(false)
{

}
