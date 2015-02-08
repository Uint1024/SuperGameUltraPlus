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
  kEditorObject_Wire_Underground_Exit,
  kEditorObject_Wire_Underground,
  kEditorObject_Constant_1,
  kEditorObject_Constant_0,
  kEditorObject_Separator,
  kEditorObject_ThreeState,
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
  kKey_ToolBar1,
  kKey_ToolBar2,
  kKey_ToolBar7,
  kKey_ToolBar9,
  kKey_And,
  kKey_Not,
  kKey_Constant_1,
  kKey_Constant_0,
  kKey_ToolBar3,
  kKey_Inventory,
  kKey_Workshop,
  kKey_Copy,
  kKey_Paste,
  kKey_Zoom_Out,
  kKey_Zoom_In,
  
  kKey_Select,
  kKey_Delete,
  kKey_Brush,
  kKey_Fill,
  kKey_Load,
  kKey_Raise,
  kKey_Lower,
  kKey_ToggleHeight,
  kKey_Select_To_Save,
  kKey_Enter,
  kKey_Backspace,
  kKey_None,
  kKey_Rotate_Left,
  kKey_Rotate_Right,
  kKey_Speed_Down,
  kKey_Speed_Up,
  kKey_Mouse_Rotate_Left,
  kKey_Mouse_Rotate_Right,
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
  kTexture_Separator,
  kTexture_And_0,
  kTexture_Not_0,
  kTexture_Separator_0,
  kTexture_And_1,
  kTexture_Not_1,
  kTexture_Separator_1,
  kTexture_Wire_Empty,
  kTexture_Wire_1,
  kTexture_Wire_0,
  kTexture_Wire_Error,
  kTexture_Wire_Underground,
  kTexture_Wire_Underground_0,
  kTexture_Wire_Underground_1,
  kTexture_Wire_Underground_SemiInvisible,
  kTexture_Wire_Underground_Exit,
  kTexture_Constant_1,
  kTexture_Constant_0,
  kTexture_ThreeState_0,
  kTexture_ThreeState_1,
  kTexture_ThreeState_Empty,
  
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

enum eColor {
  kColor_White,
  kColor_Black,
  kColor_Green,
  kColor_Orange,
  kColor_None
};
using high_resolution_clock_time_point = 
        std::chrono::time_point<std::chrono::high_resolution_clock>;

extern high_resolution_clock_time_point (*TimeNow)();

extern int CELLS_SIZE;
extern const int PIXEL_SIZE;
using milliseconds = std::chrono::milliseconds;

extern int g_delta_t;


#endif	/* UTILS_H */

