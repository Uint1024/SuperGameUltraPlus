
#include <iostream>
#include "logicgate.h"
#include "bbox.h"
#include "energy.h"
#include "and_gate.h"
#include "wire.h"
#include "static_body.h"


LogicGate::LogicGate(const Vecf& position,
        const eDirection direction, const int position_in_array,
        const Veci& map_size, const eTexture texture_id,
        const Veci& size) :
body(new StaticBody(position, size, texture_id, direction)),
position_in_map_grid(position_in_array),
logical_state(kLogicalState_Empty),
output_position_in_map_grid {-1,-1}{
  
}

void 
LogicGate::CheckOutputToWires(std::vector<Energy*>& energy_map, 
        const Veci& map_size) {
  if(output_position_in_map_grid[0] != -1 &&
          !energy_map[output_position_in_map_grid[0]]){
    if(logical_state != kLogicalState_Empty){
      energy_map[output_position_in_map_grid[0]] = 
              new Energy(logical_state);
    }
  }
  if(output_position_in_map_grid[1] != -1 &&
          !energy_map[output_position_in_map_grid[1]]){
    if(logical_state != kLogicalState_Empty){
      energy_map[output_position_in_map_grid[1]] = 
              new Energy(logical_state);
    }
  }
}

