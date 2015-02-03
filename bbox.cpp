#include <iostream>
#include "bbox.h"

Bbox::Bbox(const Vecf& position, const Veci& size_) :
left(position.x)
,top(position.y)
,right(position.x + size.x)
,bottom(position.y + size.y)
,size(size_){
  std::cout << "lol" << size.y << std::endl;
}
