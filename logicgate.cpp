#include <cstring>
#include <stdio.h>
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
        const Veci& size, const eEditorObject object_type) :
body(new StaticBody(position, size, texture_id, direction)),
position_in_map_grid(position_in_array),
logical_state(kLogicalState_Empty),
output_position_in_map_grid {-1,-1,-1},
output_direction{direction, direction, direction},
object_type(object_type),
energy_value(100){

}

LogicGate::~LogicGate() 
{
  delete body;
  body = nullptr;
}

LogicGate::LogicGate(const LogicGate& rhs) :
body(new StaticBody(Vecf{rhs.body->bbox.top, rhs.body->bbox.left}, 
        rhs.body->bbox.size, 
        rhs.body->sprite.texture_id, rhs.body->direction))
        ,position_in_map_grid(rhs.position_in_map_grid),
logical_state(kLogicalState_Empty),
object_type(rhs.object_type)
{
  std::memcpy(input_position_in_map_grid, rhs.input_position_in_map_grid, 2);
  std::memcpy(output_position_in_map_grid, rhs.output_position_in_map_grid, 3);
  std::memcpy(output_direction, rhs.output_direction, 3);
}

LogicGate& LogicGate::operator=(const LogicGate& rhs){
  //delete body;
  
  body = new StaticBody(Vecf{rhs.body->bbox.top, rhs.body->bbox.left}, 
        rhs.body->bbox.size,
          
        rhs.body->sprite.texture_id,
          
        rhs.body->direction);
  
  
  position_in_map_grid = rhs.position_in_map_grid;
  logical_state = rhs.logical_state;
  object_type = rhs.object_type;
  std::memcpy(input_position_in_map_grid, rhs.input_position_in_map_grid, 2);
  std::memcpy(output_position_in_map_grid, rhs.output_position_in_map_grid, 3);
  std::memcpy(output_direction, rhs.output_direction, 3);
  
  return *this;
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
                new Energy(logical_state, energy_value);
       }
      }
    }
  }
}

void LogicGate::CheckOutputToUndergroundWires(
        std::vector<std::array<Energy*, 4> >& energy_map, 
        std::vector<std::array<Energy*, 4> >& temp_energy_map, 
        const Veci& map_size, 
        std::vector<Wire*>& wire_map, std::vector<Wire*>& wire_map_underground) {
  
  for(int i = 0 ; i < 3 ; i++){
    if(output_position_in_map_grid[i] != -1){
      if(!temp_energy_map[output_position_in_map_grid[i]][(int)output_direction[i]]) {
        if(logical_state != kLogicalState_Empty){
          if(wire_map_underground[output_position_in_map_grid[i]] &&
                  wire_map_underground[output_position_in_map_grid[i]]->output_direction == output_direction[i]){
              temp_energy_map[output_position_in_map_grid[i]][(int)output_direction[i]] = 
                new Energy(logical_state, energy_value);
          }
       }
      }
    }
  }
}
