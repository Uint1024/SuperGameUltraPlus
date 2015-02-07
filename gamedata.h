/* 
 * File:   gamedata.h
 * Author: John
 *
 * Created on 2 février 2015, 19:09
 */

#ifndef GAMEDATA_H
#define	GAMEDATA_H

#include <vector>
#include <array>
#include <list>
#include "logicgate.h"
#include "energy.h"
#include "save_gate_window.h"

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
          const Vecf& mouse_position_in_world,
          const std::array<bool, kKey_Count>& last_keys_down,
          const std::array<bool, 255>& last_mouse_buttons_down);
  void Update();
  void Clean();
  void ResetWiresVisitedState();
  void CreateTemporaryObject(const eEditorObject object_type, const Vecf& position);
  void TransmitEnergyValue();
  Player player;
  Veci map_size;
  //grid of energy, each cell can have 4 "energy" objects, 1 from each direction
  std::vector<std::array<Energy*, 4>> energy_map;
  std::vector<std::array<Energy*, 4>> energy_map_underground;
  std::vector<LogicGate*> battery_map;
  std::vector<LogicGate*> logic_gate_map;
  std::vector<Wire*> wire_map;
  std::vector<Wire*> wire_map_underground;
  std::vector<Wire*> temporary_wire_map_blueprints;
  
  std::vector<LogicGate*> clipboard_gates;
  std::vector<Wire*> clipboard_wires;
  std::vector<Wire*> clipboard_wires_underground;
  Veci clipboard_size;
  
  eEditorObject currently_selected_object;
  Veci mouse_grid_position;
  Vecf grid_position_position;
  
  LogicGate* temporary_gate;
  Wire* temporary_wire;
  eDirection temporary_rotation;
  bool mouse_button_pressed_last_frame;
  bool keeping_mouse_pressed;
  
  SaveGateWindow* save_gate_window;
  Veci selection_grid_position_begin;
  Veci selection_grid_position_end;
  bool selecting_area;
  
  bool making_line_of_wires;
  Veci making_line_of_wires_begin;
  Veci making_line_of_wires_begin_backup;
  int update_delay;
  int update_timer;
  bool selected_area;
  bool wanting_to_paste;
};

#endif	/* GAMEDATA_H */

