/* 
 * File:   utils.h
 * Author: John
 *
 * Created on 2 février 2015, 18:40
 */

#ifndef UTILS_H
#define	UTILS_H
#include <chrono>

struct Vecf {
  float x, y;
};

struct Veci {
  int x, y;
};

enum eKey {
  kKey_Up,
  kKey_Down,
  kKey_Left,
  kKey_Right,
  kKey_And,
  kKey_Or,
  kKey_Inventory,
  kKey_Workshop,
  kKey_Copy,
  kKey_Paste,
  kKey_Select,
  kKey_Delete,
  kKey_Raise,
  kKey_Lower,
  kKey_ToggleHeight,
  kKey_None,
  kKey_Count
};

enum eTexture {
  kTexture_Player,
  kTexture_And,
  kTexture_Or,
  kTexture_Wire,
  kTexture_None,
  kTexture_Count
};

enum eDirection {
  kDirection_Up,
  kDirection_Down,
  kDirection_Left,
  kDirection_Right,
  kDirection_Count
};

using high_resolution_clock_time_point = 
        std::chrono::time_point<std::chrono::high_resolution_clock>;

extern high_resolution_clock_time_point (*TimeNow)();

using milliseconds = std::chrono::milliseconds;

extern int g_delta_t;


#endif	/* UTILS_H */

