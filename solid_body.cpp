#include "solid_body.h"
#include "engine.h"

SolidBody::SolidBody(const Vecf& position, const Veci& size, 
        const eTexture texture_id, const eDirection direction) :
bbox(position, size),
sprite(texture_id),
direction(direction){
  
}

