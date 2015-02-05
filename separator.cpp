#include <iostream>
#include "separator.h"
#include "and_gate.h"
#include "energy.h"

Separator::Separator(const Vecf& position,
        const eDirection direction, const int position_in_array, 
        const Veci& map_size) :
LogicGate(position, direction, position_in_array, map_size, kTexture_Separator,
        Veci{32,32}){
    if(direction == kDirection_Down) {
      output_position_in_map_grid[0] = position_in_array + map_size.x * 2;
      output_position_in_map_grid[1] = position_in_array + map_size.x * 2 +1;
      input_position_in_map_grid[0] = position_in_array + 1;
    }
    if(direction == kDirection_Left) {
      
      output_position_in_map_grid[0] = position_in_array - 1;
      output_position_in_map_grid[1] = position_in_array + map_size.x * 2 - 1;
      input_position_in_map_grid[0] = position_in_array + map_size.x + 2;
    }
    if(direction == kDirection_Right) {
      output_position_in_map_grid[0] = position_in_array + 3;
      output_position_in_map_grid[1] = position_in_array + map_size.x * 2 + 3;
      input_position_in_map_grid[0] = position_in_array + map_size.x;
    }
    if(direction == kDirection_Up) {
      output_position_in_map_grid[0] = position_in_array - map_size.x ;
      output_position_in_map_grid[1] = position_in_array - map_size.x  + 2;
      input_position_in_map_grid[0] = position_in_array + map_size.x * 2 + 1;
    }
}

void Separator::RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) {
  /*if(energy_map[input_position_in_map_grid[0]]){
    logical_state = energy_map[input_position_in_map_grid[0]]->state;
  }
  else{
    logical_state = kLogicalState_Empty;
  }

  delete energy_map[input_position_in_map_grid[0]];
  energy_map[input_position_in_map_grid[0]] = nullptr;*/
}

