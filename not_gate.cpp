#include "not_gate.h"
#include "energy.h"
NotGate::NotGate(const Vecf& position, const eDirection direction, 
        const int position_in_array, const Veci& map_size):
LogicGate(position, direction, position_in_array, map_size, kTexture_Not) {
  output_position_in_map_grid = position_in_array + map_size.x * 3 + 1;
  input_position_in_map_grid[0] = position_in_array + 1;
}

void 
NotGate::RunLogic(std::vector<Energy*>& energy_map) {
  if(!energy_map[input_position_in_map_grid[0]]){
    logical_state = kLogicalState_Empty;
  }
  else if(energy_map[input_position_in_map_grid[0]]->state == kLogicalState_1) {
      logical_state = kLogicalState_0;
  } else {
      logical_state = kLogicalState_1;
  }
  

  delete energy_map[input_position_in_map_grid[0]];
  energy_map[input_position_in_map_grid[0]] = nullptr;
}

