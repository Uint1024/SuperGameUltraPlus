/* 
 * File:   gamedata.h
 * Author: John
 *
 * Created on 2 février 2015, 19:09
 */

#ifndef GAMEDATA_H
#define	GAMEDATA_H

#include <vector>
#include "logicgate.h"
#include "energy.h"

#include "player.h"
class Wire;
class Engine;

struct GameData{
  GameData(const Veci& window_size);
  void InitializeWorld(const Veci& window_size);
  void Render(Engine& engine);
  Vecf ReceiveInput(const std::array<bool, kKey_Count>& keys_down, 
          const std::array<bool, 255>& mouse_buttons_down,
          const Veci& mouse_position_in_window,
          const Vecf& mouse_position_in_world);
  void Update();
  void Clean();
  void ResetWiresVisitedState();
  Player player;
  Veci map_size;
  std::vector<Energy*> energy_map;
  std::vector<LogicGate*> battery_map;
  std::vector<LogicGate*> logic_gate_map;
  std::vector<Wire*> wire_map;
  std::vector<Wire*> temporary_wire_map_blueprints;
  eEditorObject currently_selected_object;
  Veci mouse_grid_position;
  Vecf grid_position_position;
  
  LogicGate* temporary_gate;
  Wire* temporary_wire;
  eDirection temporary_rotation;
  bool mouse_button_pressed_last_frame;
  bool keeping_mouse_pressed;
  Veci mouse_selection_grid_position_begin;
  Veci mouse_selection_grid_position_end;
  bool selecting_area;
};

#endif	/* GAMEDATA_H */

