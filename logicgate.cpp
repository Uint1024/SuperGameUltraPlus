
#include <iostream>
#include "logicgate.h"
#include "bbox.h"
#include "energy.h"
#include "and_gate.h"
#include <iostream>
#include "wire.h"
#include "static_body.h"


LogicGate::LogicGate(const Vecf& position,
        const eDirection direction, const int position_in_array,
        const Veci& map_size, const eTexture texture_id,
        const Veci& size) :
body(new StaticBody(position, size, texture_id, direction)),
position_in_map_grid(position_in_array),
logical_state(kLogicalState_Empty),
output_position_in_map_grid {-1,-1,-1},
output_direction{direction, direction, direction}{
  
}

LogicGate::~LogicGate(){
  delete body;
  body = nullptr;
}

void 
LogicGate::CheckOutputToWires(std::vector<std::array<Energy*, 4>>& energy_map, 
        std::vector<std::array<Energy*, 4>>& temp_energy_map,
        const Veci& map_size) {

  for(int i = 0 ; i < 3 ; i++){
    if(output_position_in_map_grid[i] != -1){
      if(!temp_energy_map[output_position_in_map_grid[i]][(int)output_direction[i]]) {
        if(logical_state != kLogicalState_Empty){
          temp_energy_map[output_position_in_map_grid[i]][(int)output_direction[i]] = 
                new Energy(logical_state);
       }
      }
    }
  }
  

}

