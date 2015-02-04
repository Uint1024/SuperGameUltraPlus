
#include <iostream>
#include "logicgate.h"
#include "bbox.h"
#include "energy.h"
#include "and_gate.h"
#include "wire.h"
#include "static_body.h"


LogicGate::LogicGate(const Vecf& position,
        const eDirection direction, const int position_in_array,
        const Veci& map_size, const eTexture texture_id) :
body(new StaticBody(position, Veci{48,48}, texture_id, direction)),
position_in_map_grid(position_in_array),
logical_state(kLogicalState_Empty){
  
}

void 
LogicGate::CheckOutputToWires(std::vector<Energy*>& energy_map, 
        const Veci& map_size) {
  if(!energy_map[output_position_in_map_grid]){
    
    if(logical_state != kLogicalState_Empty){
      energy_map[output_position_in_map_grid] = 
              new Energy(logical_state);
    }
  }
}

