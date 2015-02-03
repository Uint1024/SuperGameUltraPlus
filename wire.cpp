#include <iostream>
#include "wire.h"
#include "solid_body.h"
Wire::Wire(const Vecf& position) :
body(position, Veci{16,16}, kTexture_Wire_Empty),
parent(nullptr){
  
}