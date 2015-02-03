
#include <iostream>
#include "logicgate.h"

LogicGate::LogicGate(const Vecf& position, const eEditorObject type) :
body(position, Veci{48,48}, kTexture_None){
  if(type == kEditorObject_Constant_1){
    body.sprite.texture_id = kTexture_Constant_1;
  }
  
}

