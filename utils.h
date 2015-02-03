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

