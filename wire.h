/* 
 * File:   Wire.h
 * Author: John
 *
 * Created on 3 février 2015, 04:41
 */

#ifndef WIRE_H
#define	WIRE_H
#include <vector>
#include <array>
#include "solid_body.h"

class Engine;
class Energy;
struct Wire {
  Wire(const Vecf& position, const int position_in_array,
          const eDirection direction,
          const eEditorObject type);
  ~Wire();
  void ChangeState (Wire* parent, eLogicalState state, 
                      std::vector<Wire*>& wire_map,
                      const Veci& map_size);
  void CheckOutputToWire(std::vector<Wire*>& wire_map);
  void MoveElectrons(std::vector<Wire*>& wire_map, const Veci& map_size);
  void CheckIfHasEnergy(std::vector<std::array<Energy*, 4>>& energy_map);
  void Rotate(const eDirection direction);
  void ResetState();
  int position_in_array;
  SolidBody* body;
  Wire* parent;
  eLogicalState logical_state;
  //wires can only be visited once per frame to prevent infinite loop
  bool visited;
  eEditorObject type;
  eDirection output_direction;
  int energy_value;
  bool visible;
  
};

#endif	/* WIRE_H */

