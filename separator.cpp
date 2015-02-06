#include <iostream>
#include "separator.h"
#include "and_gate.h"
#include "energy.h"

Separator::Separator(const Vecf& position,
        const eDirection direction, const int position_in_array, 
        const Veci& map_size) :
LogicGate(position, direction, position_in_array, map_size, kTexture_Separator,
        Veci{CELLS_SIZE,CELLS_SIZE}, kEditorObject_Separator){
          Rotate(direction, map_size);
}
void Separator::Rotate(const eDirection direction, const Veci& map_size) {
  
  body->direction = direction;
  input_position_in_map_grid[0] = position_in_map_grid;
  if(direction == kDirection_Down) {
    output_direction[2] = kDirection_Down;
    output_direction[0] = kDirection_Left;
    output_direction[1] = kDirection_Right;
    output_position_in_map_grid[0] = position_in_map_grid - 1;
    output_position_in_map_grid[1] = position_in_map_grid + 1;
    output_position_in_map_grid[2] = position_in_map_grid + map_size.x; 
  }
  if(direction == kDirection_Left) {
    output_direction[1] = kDirection_Down;
    output_direction[0] = kDirection_Left;
    output_direction[2] = kDirection_Up;
    output_position_in_map_grid[0] = position_in_map_grid - map_size.x;
    output_position_in_map_grid[1] = position_in_map_grid + map_size.x;
    output_position_in_map_grid[2] = position_in_map_grid - 1 ; 
  }
  if(direction == kDirection_Right) {
    output_direction[1] = kDirection_Down;
    output_direction[0] = kDirection_Up;
    output_direction[2] = kDirection_Right;
          output_position_in_map_grid[0] = position_in_map_grid - map_size.x;
    output_position_in_map_grid[1] = position_in_map_grid + map_size.x;
    output_position_in_map_grid[2] = position_in_map_grid + 1; 
  }
  if(direction == kDirection_Up) {
    output_direction[2] = kDirection_Up;
    output_direction[0] = kDirection_Left;
    output_direction[1] = kDirection_Right;
    output_position_in_map_grid[0] = position_in_map_grid - 1;
    output_position_in_map_grid[1] = position_in_map_grid + 1;
    output_position_in_map_grid[2] = position_in_map_grid - map_size.x; 
  }
}

void Separator::RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) {
  int high_energy = 0;
  int low_energy = 0;
  bool error = false;
  
  for(int i = 0 ; i < 4 ; i++) {
    if(energy_map[input_position_in_map_grid[0]][i]) {
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_0) {
        ++low_energy;
      }
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_1) {
         
        ++high_energy;
      }
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_Error) {
        error = true;
      }
    }
  }
  
    
  if(low_energy == 0 && high_energy == 0){
    logical_state = kLogicalState_Empty;
  }
  else if(low_energy > 0 && high_energy == 0){
    logical_state = kLogicalState_0;
  }
  else if(low_energy == 0 && high_energy > 0){
    logical_state = kLogicalState_1;
  }
  else if((low_energy > 0 && high_energy > 0) || error){
    logical_state == kLogicalState_Error;
  }
  
    switch(logical_state){
    case kLogicalState_0:
      body->sprite.texture_id = kTexture_Separator_0;
      break;
    case kLogicalState_1:
      body->sprite.texture_id = kTexture_Separator_1;
      break;
    default:
      body->sprite.texture_id = kTexture_Separator;
      break;
  }
}

