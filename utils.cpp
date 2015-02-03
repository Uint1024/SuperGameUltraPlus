#include "utils.h"

void
AddVecf(const Vecf& to_add, Vecf* base){
  base->x += to_add.x;
  base->y += to_add.y;
}
