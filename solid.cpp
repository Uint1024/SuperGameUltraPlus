#include "solid.h"


Solid::Solid(const Vecf& pos, const Veci& size) :
pos(pos)
,bbox(pos, size){
  
}

