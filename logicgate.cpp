
#include <iostream>
#include "logicgate.h"
#include "bbox.h"
#include "energy.h"
#include "and.h"
#include "wire.h"
#include "static_body.h"


LogicGate::LogicGate(const Vecf& position, const eEditorObject type,
        const eDirection direction, const int position_in_array,
        const Veci& map_size, std::vector<Wire*>& wire_map) :
body(new StaticBody(position, Veci{48,48}, kTexture_None, direction)),
output_wire(nullptr),
position_in_map_grid(position_in_array){
  if(type == kEditorObject_Constant_1){
    body->sprite.texture_id = kTexture_Constant_1;
    logical_state = kLogicalState_1;
    output_position_in_map_grid = position_in_array + map_size.x * 3 + 1;
    /*output_wire = new Wire(Vecf{position.x + 16, position.y + 32}, 
            output_position_in_map_grid, kDirection_Down);
    
    output_wire->logical_state = logical_state;
    wire_map[output_position_in_map_grid] = output_wire;*/
    /*parts_map = 
      { kLogicGatePart_Neutral, kLogicGatePart_Neutral, kLogicGatePart_Neutral,
        kLogicGatePart_Neutral, kLogicGatePart_Neutral, kLogicGatePart_Neutral,
        kLogicGatePart_Neutral, kLogicGatePart_Output, kLogicGatePart_Neutral };
    
    output_bbox = new Bbox(Vecf{position.x + 16, position.y + 32}, Veci{16,16});
     */
  }
}

void 
LogicGate::CheckOutputToWires(std::vector<Energy*>& energy_map, 
        const Veci& map_size) {
  if(!energy_map[output_position_in_map_grid]){
    //std::cout << output_position_in_map_grid << std::endl;
    energy_map[output_position_in_map_grid] = 
            new Energy(logical_state);
  }
}

