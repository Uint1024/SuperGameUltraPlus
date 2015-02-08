#include <iostream>
#include "three_state.h"
#include "energy.h"

ThreeState::ThreeState(const Vecf& position,
        const eDirection direction, const int position_in_array, 
        const Veci& map_size) :
LogicGate(position, direction, position_in_array, map_size, kTexture_ThreeState_Empty, 
        Veci{CELLS_SIZE,CELLS_SIZE}, kEditorObject_ThreeState){
  Rotate(direction, map_size);
}

void ThreeState::Rotate(const eDirection direction, const Veci& map_size) {
  body->direction = direction;
  input_position_in_map_grid[0] = position_in_map_grid;
  input_position_in_map_grid[1] = position_in_map_grid;
    output_direction[0] = direction;
  if(direction == kDirection_Down){
    output_position_in_map_grid[0] = position_in_map_grid + map_size.x; 
  }
  if(direction == kDirection_Left){
    
    output_position_in_map_grid[0] = position_in_map_grid - 1 ;
  }
  if(direction == kDirection_Right){
    output_position_in_map_grid[0] = position_in_map_grid + 1 ;
  }
  if(direction == kDirection_Up){
    output_position_in_map_grid[0] = position_in_map_grid - map_size.x;
  }
}

void ThreeState::RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) {
  bool has_high_energy = false;
  bool has_low_energy = false;
  
  int high_energy = 0;
  int low_energy = 0;
  bool error = false;
  
  //"normal" input
  if(energy_map[input_position_in_map_grid[0]][(int)output_direction[0]]) {
    if(energy_map[input_position_in_map_grid[0]][(int)output_direction[0]]->state == kLogicalState_0) {
      ++low_energy;
    }
    if(energy_map[input_position_in_map_grid[0]][(int)output_direction[0]]->state == kLogicalState_1) {
      ++high_energy;
    }
    if(energy_map[input_position_in_map_grid[0]][(int)output_direction[0]]->state == kLogicalState_Error) {
      error = true;
    }
  }
  
  bool output = false;
  

  //three state logic input, determine if gate will output anything
  for(int i = 0 ; i < 4 ; i++) {
    if(i != (int)output_direction[0])
    if(energy_map[input_position_in_map_grid[0]][i]) {
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_0) {
        output = false;
      }
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_1) {
        output = true;
      }
      if(energy_map[input_position_in_map_grid[0]][i]->state == kLogicalState_Error) {
        error = true;
        output = false;
      }
    }
  }
  
  if(output){
    if(high_energy > 0 && low_energy == 0){
      logical_state = kLogicalState_1;
    }
    if(high_energy == 0 && low_energy > 0){
      logical_state = kLogicalState_0;
    }
    if((high_energy > 0 && low_energy > 0) || error){
      logical_state == kLogicalState_Error;
    }
    if(high_energy == 0 && low_energy == 0){
      logical_state = kLogicalState_Empty;
    }
  }
  else {
    logical_state = kLogicalState_Empty;
  }
  
  switch(logical_state){
    case kLogicalState_0:
      body->sprite.texture_id = kTexture_ThreeState_0;
      break;
    case kLogicalState_1:
      body->sprite.texture_id = kTexture_ThreeState_1;
      break;
    default:
      body->sprite.texture_id = kTexture_ThreeState_Empty;
      break;
  }

  for(int i = 0 ; i < 4 ; i++) {
    delete energy_map[input_position_in_map_grid[0]][i];
    energy_map[input_position_in_map_grid[0]][i] = nullptr;
  }
}

