#include <iostream>
#include "wire.h"
#include "solid_body.h"
#include "static_body.h"
Wire::Wire(const Vecf& position, const int position_in_array, 
        const eDirection direction) :
body(new StaticBody(position, Veci{16,16}, kTexture_Wire_Empty, direction)),
parent(nullptr),
logical_state(kLogicalState_Empty),
position_in_array(position_in_array),
visited(false){
  
}

void 
Wire::ChangeState(Wire* parent, eLogicalState state, 
        std::vector<Wire*>& wire_map,
        const Veci& map_size) {
  visited = true;
  parent = parent;
  logical_state = state;
  switch(state){
    case kLogicalState_0:
      body->sprite.texture_id = kTexture_Wire_0;
      break;
    case kLogicalState_1:
      body->sprite.texture_id = kTexture_Wire_1;
      break;
    case kLogicalState_Empty:
      body->sprite.texture_id = kTexture_Wire_Empty;
      break;
    default:
      std::cout << "Error, unknown state." << std::endl;
      break;
  }
  
  /*for(auto &wire : wire_map){
    if(wire){
      if(wire != this && wire != parent && 
              wire->body.bbox.CollisionWithBbox(body.bbox)){
        wire->ChangeState(this, logical_state, wire_map);
      }
    }
  }*/
  Wire* wire_right = nullptr;
  Wire* wire_up = nullptr;
  Wire* wire_left = nullptr;
  Wire* wire_down = nullptr;
  switch(body->direction){
    case kDirection_Right:
      if(position_in_array + 1 < wire_map.size()) {
        wire_right = wire_map[position_in_array + 1];
      }
      if(wire_right && !wire_right->visited && wire_right != parent){
        wire_right->ChangeState(this, logical_state, wire_map, map_size);        
      }
      break;
    case kDirection_Up:
      if(position_in_array - map_size.x > 0) {
        wire_up = wire_map[position_in_array - map_size.x];
      }
      if(wire_up && !wire_up->visited && wire_up != parent){
        wire_up->ChangeState(this, logical_state, wire_map, map_size);        
      }
      break;
    case kDirection_Left:
      if(position_in_array - 1 < wire_map.size()) {
        wire_left = wire_map[position_in_array - 1];
      }
      if(wire_left && !wire_left->visited && wire_left != parent){
        wire_left->ChangeState(this, logical_state, wire_map, map_size);        
      }
      break;
    case kDirection_Down:
      if(position_in_array + map_size.x < wire_map.size()) {
        wire_down = wire_map[position_in_array + map_size.x];
      }
      if(wire_down && !wire_down->visited && wire_down != parent){
        wire_down->ChangeState(this, logical_state, wire_map, map_size);        
      }
      break;
    
  }
  
    /*
    Wire* wire_left = nullptr;
    Wire* wire_top = nullptr;
    Wire* wire_bottom = nullptr;
      
    
     
    if(position_in_array - 1 > 0)
      wire_left = wire_map[position_in_array - 1];
    
    
    
    
    
    if(wire_right){
      
      if(wire_right->logical_state != logical_state && wire_right != parent){
        
        wire_right->ChangeState(this, logical_state, wire_map, map_size);        
      }
    }
    */
    /*if(wire_left){
      if(wire_left->logical_state != logical_state && wire_left != parent){
        wire_left->ChangeState(this, logical_state, wire_map, map_size);        
      }
    }
    if(wire_top){
      if(wire_top->logical_state != logical_state && wire_top != parent){
        wire_top->ChangeState(this, logical_state, wire_map, map_size);        
      }
    }
    if(wire_bottom){
      if(wire_bottom->logical_state != logical_state && wire_bottom != parent){
        wire_bottom->ChangeState(this, logical_state, wire_map, map_size);        
      }
    }*/
  
}

void 
Wire::CheckOutputToWire(std::vector<Wire*> wire_map) {
  
}

void 
Wire::ResetState() {
  visited = false;
  parent = nullptr;
  logical_state = kLogicalState_Empty;
  body->sprite.texture_id = kTexture_Wire_Empty;;
}
