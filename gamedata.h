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
  Player player;
  Veci map_size;
  std::vector<LogicGate*> logic_gate_map;
  std::vector<Wire*> wire_map;
};

#endif	/* GAMEDATA_H */

