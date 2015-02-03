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

struct GameData{
  GameData();
  Player player;
  std::vector<LogicGate> logic_gates_map;
};

#endif	/* GAMEDATA_H */

