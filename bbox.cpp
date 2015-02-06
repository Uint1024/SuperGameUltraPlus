#include <iostream>
#include "bbox.h"

Bbox::Bbox(const Vecf& position, const Veci& size) :
left(position.x)
,top(position.y)
,right(position.x + size.x)
,bottom(position.y + size.y)
,size(size){
}

void
Bbox::Move(const Vecf& movement) {
  left += movement.x;
  right += movement.x;
  top += movement.y;
  bottom += movement.y;
}

void 
Bbox::MoveTo(const Vecf& position) {
  left = position.x;
  right = position.x + size.x;
  top = position.y;
  bottom = position.y  + size.y;
}

bool 
Bbox::CollisionWithPoint(const Vecf& point) {
  if(left > point.x || right < point.x || 
     top > point.y || bottom < point.y) {
    return false;
  }
  
  return right;
}

bool 
Bbox::CollisionWithBbox(const Bbox& other) {
  if(other.left < right && other.bottom > top && 
          other.right > left && other.top < bottom){
    return true;
  }
  
  return false;
}

