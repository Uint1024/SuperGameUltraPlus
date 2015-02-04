
#include <iostream>
#include "logicgate.h"
#include "bbox.h"
#include "and.h"
#include "wire.h"
#include "static_body.h"


LogicGate::LogicGate(const Vecf& position, const eEditorObject type,
        const eDirection direction) :
body(new StaticBody(position, Veci{48,48}, kTexture_None, direction)),
output_bbox(nullptr){
  if(type == kEditorObject_Constant_1){
    body->sprite.texture_id = kTexture_Constant_1;
    logical_state = kLogicalState_1;
    parts_map = 
      { kLogicGatePart_Neutral, kLogicGatePart_Neutral, kLogicGatePart_Neutral,
        kLogicGatePart_Neutral, kLogicGatePart_Neutral, kLogicGatePart_Neutral,
        kLogicGatePart_Neutral, kLogicGatePart_Output, kLogicGatePart_Neutral };
    
    output_bbox = new Bbox(Vecf{position.x + 16, position.y + 32}, Veci{16,16});
  }
  
  
  
}

void 
LogicGate::CheckOutputToWires(std::vector<Wire*>& wire_map, 
        const Veci& map_size) {
  for(auto &wire : wire_map){
    if(wire){
      if(output_bbox->CollisionWithBbox(wire->body->bbox)){
        wire->ChangeState(nullptr, logical_state, wire_map, map_size);
      }
    }
  }
}

