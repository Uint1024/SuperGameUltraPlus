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

void AddVecf(const Vecf& to_add, Vecf* base);

enum eEditorObject {
  kEditorObject_And,
  kEditorObject_Not,
  kEditorObject_Wire,
  kEditorObject_Constant_1,
  kEditorObject_Constant_0,
  kEditorObject_Separator,
  kEditorObject_None,
  kEditorObject_Count
};
enum eGateType {
  kGateType_And,
  kGateType_Or,
  kGateType_Constant_1,
  kGateType_Constant_0
};

enum eConnection {
  kConnection_In,
  kConnection_Out,
  kConnection_Neutral
};

enum eKey {
  kKey_Up,
  kKey_Down,
  kKey_Left,
  kKey_Right,
  kKey_Wire,
  kKey_And,
  kKey_Not,
  kKey_Constant_1,
  kKey_Constant_0,
  kKey_Separator,
  kKey_Inventory,
  kKey_Workshop,
  kKey_Gate_1,
  kKey_Gate_2,
  kKey_Gate_3,
  kKey_Gate_4,
  kKey_Gate_5,
  kKey_Gate_6,
  kKey_Gate_7,
  kKey_Gate_8,
  kKey_Gate_9,
  kKey_Copy,
  kKey_Paste,
  kKey_Select,
  kKey_Delete,
  kKey_Raise,
  kKey_Lower,
  kKey_ToggleHeight,
  kKey_Select_To_Save,
  kKey_Enter,
  kKey_None,
  kKey_Rotate_Left,
  kKey_Rotate_Right,
  kKey_Speed_Down,
  kKey_Speed_Up,
  kKey_Count
};

enum eLogicGatePart {
  kLogicGatePart_Output,
  kLogicGatePart_Input,
  kLogicGatePart_Neutral,
  kLogicGatePart_Count
};

enum eLogicalState {
  kLogicalState_1,
  kLogicalState_0,
  kLogicalState_Empty,
  kLogicalState_Error,
  kLogicalState_Count
};

enum eTexture {
  kTexture_Player,
  kTexture_And,
  kTexture_Not,
  kTexture_Wire_Empty,
  kTexture_Wire_1,
  kTexture_Wire_0,
  kTexture_Wire_Error,
  kTexture_Constant_1,
  kTexture_Constant_0,
  kTexture_Separator,
  kTexture_None,
  kTexture_Count
};

enum eDirection {
  kDirection_Right,
  kDirection_Down,
  kDirection_Left,
  kDirection_Up,
  kDirection_Count
};

using high_resolution_clock_time_point = 
        std::chrono::time_point<std::chrono::high_resolution_clock>;

extern high_resolution_clock_time_point (*TimeNow)();

extern const int CELLS_SIZE;
using milliseconds = std::chrono::milliseconds;

extern int g_delta_t;


#endif	/* UTILS_H */

