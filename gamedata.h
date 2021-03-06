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
#include "SDL_ttf.h"
#include "label.h"
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
  Vecf ReceiveInput(std::string text_input, 
  const std::array<bool, kKey_Count>& keys_down, 
          const std::array<bool, 255>& mouse_buttons_down,
          const Veci& mouse_position_in_window,
          const Vecf& mouse_position_in_world,
          const std::array<bool, kKey_Count>& last_keys_down,
          const std::array<bool, 255>& last_mouse_buttons_down);
  void CreateSingleObject();
  void DeleteSingleObject(const Vecf& mouse_position_in_world);
  void CheckItemSelectionKeys(const std::array<bool, kKey_Count>& keys_down,
  const std::array<bool, kKey_Count>& last_keys_down);
  void UpdateMousePosition(const Vecf& mouse_position_in_world);
  void DeleteSelection();
  void CopySelectionToClipboard();
  void MoveClipboardObjects();
  void PasteClipboardObjects();
  void CreateLineOfWires();
  
  void CheckMouseObjectsCollision(const std::array<bool, kKey_Count>& keys_down,
                        const std::array<bool, 255 >& mouse_buttons_down, 
                        const Veci& mouse_position_in_window,
                        const Vecf& mouse_position_in_world,
                        const std::array<bool, kKey_Count>& last_keys_down,
        const std::array<bool, 255>& last_mouse_buttons_down);
  void Update();
  void Clean();
  void BrushMode( const std::array<bool, kKey_Count>& keys_down, 
                      const std::array<bool, 255 > & mouse_buttons_down, 
                        const Veci& mouse_position_in_window,
                        const Vecf& mouse_position_in_world,
                        const std::array<bool, kKey_Count>& last_keys_down,
        const std::array<bool, 255>& last_mouse_buttons_down);
  void ResetWiresVisitedState();
  void CreateTemporaryObject(const eEditorObject object_type, const Vecf& position);
  void TransmitEnergyValue();
  void RenderColorSquare(Engine& engine, const eColor ecolor, 
        const int x, const int y);
  void Save(std::string file_name);
  void Load(std::string file_name);
  inline void CheckUniqueKeyPress();
  Player player;
  Veci map_size;
  Veci map_size_pixels;
  //grid of energy, each cell can have 4 "energy" objects, 1 from each direction
  std::vector<std::array<Energy*, 4>> energy_map;
  std::vector<std::array<Energy*, 4>> energy_map_underground;
  std::vector<LogicGate*> battery_map;
  std::vector<LogicGate*> logic_gate_map;
  std::vector<Wire*> wire_map;
  std::vector<Wire*> wire_map_underground;
  std::vector<Wire*> temporary_wire_map_blueprints;
  std::vector<eColor> color_map;
  std::vector<eColor> clipboard_color_map;
  
  std::vector<Label> labels;
  bool create_label_mode;
  
  std::vector<LogicGate*> clipboard_gates;
  std::vector<Wire*> clipboard_wires;
  std::vector<Wire*> clipboard_wires_underground;
  Veci clipboard_size;
  
  eEditorObject currently_selected_object;
  Veci mouse_grid_position;
  int mouse_vector_position;
  Vecf grid_position_position;
  Vecf mouse_world_position;
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
  
  bool paste_mode;
  bool brush_mode;
  eColor brush_color;
  
  bool mouse_collide_with_object;
  bool pressed_rotate;
  
  std::string temporary_label_string;
  Label* temporary_label;
  Vecf temporary_label_position;
  
  std::map<eKey, eEditorObject> last_selected_toolbar_item;
};

#endif	/* GAMEDATA_H */

