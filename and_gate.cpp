#include <iostream>
#include "and_gate.h"
#include "energy.h"

AndGate::AndGate(const Vecf& position,
        const eDirection direction, const int position_in_array, 
        const Veci& map_size) :
LogicGate(position, direction, position_in_array, map_size, kTexture_And, Veci{48,48}){
  output_position_in_map_grid[0] = position_in_array + map_size.x * 3 + 1;
  input_position_in_map_grid[0] = position_in_array;
  input_position_in_map_grid[1] = position_in_array + 2;
}

void AndGate::RunLogic(std::vector<Energy*>& energy_map) {
  if(!energy_map[input_position_in_map_grid[0]] &&
          !energy_map[input_position_in_map_grid[1]]){
    logical_state = kLogicalState_Empty;
  }
  else if(energy_map[input_position_in_map_grid[0]] &&
          energy_map[input_position_in_map_grid[1]]) {
    if(energy_map[input_position_in_map_grid[0]]->state == kLogicalState_1 &&
          energy_map[input_position_in_map_grid[1]]->state == kLogicalState_1) {
      logical_state = kLogicalState_1;
    }
    else {
      logical_state = kLogicalState_0;
    }
  }
  
  if((energy_map[input_position_in_map_grid[0]] &&
          !energy_map[input_position_in_map_grid[1]]) ||
     (!energy_map[input_position_in_map_grid[0]] &&
          energy_map[input_position_in_map_grid[1]])){
    std::cout << "heh" << std::endl;
            
    logical_state = kLogicalState_0;
  }

  delete energy_map[input_position_in_map_grid[0]];
  energy_map[input_position_in_map_grid[0]] = nullptr;
  delete energy_map[input_position_in_map_grid[1]];
  energy_map[input_position_in_map_grid[1]] = nullptr;
}
