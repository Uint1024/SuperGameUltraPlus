
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
output_position_in_map_grid {-1,-1},
output_direction(direction){
  
}

void 
LogicGate::CheckOutputToWires(std::vector<std::array<Energy*, 4>>& energy_map, 
        const Veci& map_size) {
  //std::cout << output_position_in_map_grid[0] << std::endl;
  //check if there's energy of the same type at the output cell
  bool there_is_energy_at_output_already = false;
  
  if(output_position_in_map_grid[0] != -1) {
    Energy* energy_at_first_output = 
            energy_map[output_position_in_map_grid[0]][(int)output_direction];
    
    if(!energy_map[output_position_in_map_grid[0]][(int)output_direction]){
        
      if(logical_state != kLogicalState_Empty){
        energy_map[output_position_in_map_grid[0]][(int)output_direction] = 
                new Energy(logical_state);
        
      }
    }
  }
  
  if(output_position_in_map_grid[1] != -1){
    Energy* energy_at_second_output = 
          energy_map[output_position_in_map_grid[1]][(int)output_direction];
 
    if(!energy_map[output_position_in_map_grid[1]][(int)output_direction]) {
      if(logical_state != kLogicalState_Empty){
        energy_map[output_position_in_map_grid[1]][(int)output_direction] = 
                new Energy(logical_state);
      }
    }
  }
  
  
}

