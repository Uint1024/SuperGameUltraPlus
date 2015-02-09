/* 
 * File:   logicgate.h
 * Author: John
 *
 * Created on 2 février 2015, 19:12
 */

#ifndef LOGICGATE_H
#define	LOGICGATE_H
#include <array>
#include <vector>
#include <map>


#include "utils.h"
#include "solid_body.h"
class Wire;
class Energy;

struct LogicGate {
  LogicGate(const Vecf& position,
            const eDirection direction, const int position_in_array,
            const Veci& map_size, const eTexture texture_id,
            const Veci& size, const eEditorObject object_type);
  ~LogicGate();
  LogicGate(const LogicGate& rhs);
  void CheckOutputToWires(std::vector<std::array<Energy*, 4>>& energy_map, 
  std::vector<std::array<Energy*, 4>>& temp_energy_map, const Veci& map_size);
  void CheckOutputToUndergroundWires(
      std::vector<std::array<Energy*, 4>>& energy_map, 
      std::vector<std::array<Energy*, 4>>& temp_energy_map, 
      const Veci& map_size,
      std::vector<Wire*>& wire_map,
      std::vector<Wire*>& wire_map_underground);
  virtual void Rotate(const eDirection direction, const Veci& map_size) = 0;
  LogicGate& operator=(const LogicGate& rhs);
  SolidBody* body;
  eGateType type;
  eLogicalState logical_state;
  virtual void RunLogic(std::vector<std::array<Energy*, 4>>& energy_map) = 0;

  //this is the top left corner of the gate in the map grid
  int position_in_map_grid;
  //position of the wire it will output to
  int output_position_in_map_grid[3];
  int input_position_in_map_grid[2];
  eDirection output_direction[3];
  eEditorObject object_type;
  int energy_value;
};

#endif	/* LOGICGATE_H */

