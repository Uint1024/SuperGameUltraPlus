#include "and_gate.h"

AndGate::AndGate(const Vecf& position,
        const eDirection direction, const int position_in_array, 
        const Veci& map_size) :
LogicGate(position, direction, position_in_array, map_size, kTexture_And){
/*if(type == kEditorObject_Constant_1){
    body->sprite.texture_id = kTexture_Constant_1;
    logical_state = kLogicalState_1;
    output_position_in_map_grid = position_in_array + map_size.x * 3 + 1;
  }*/

  output_position_in_map_grid = position_in_array + map_size.x * 3 + 1;
}

void AndGate::RunLogic() {

}
