#ifndef LUAMAGUS_H
#define LUAMAGUS_H

/// @file LuaMagus.h
/// @brief LuaMagus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"
#include "ym/LuaBnet.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class LuaMagus LuaMagus.h "LuaMagus.h"
/// @brief Magus 用の Luapp 拡張
//////////////////////////////////////////////////////////////////////
class LuaMagus :
  public LuaBnet
{
public:

  /// @brief 空のコンストラクタ
  LuaMagus() = default;

  /// @brief 通常のコンストラクタ
  LuaMagus(
    lua_Alloc f, ///< [in] アロケーター
    void* ud     ///< [in] ユーザーデータ
  ) : LuaBnet{f, ud}
  {
  }

  /// @brief すでに生成済みのインタプリタを用いるコンストラクタ
  LuaMagus(
    lua_State* L ///< [in] lua インタープリタ
  ) : LuaBnet{L}
  {
  }

  /// @brief デストラクタ
  ~LuaMagus() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  ///@ brief Magus 拡張に関する初期化を行う．
  ///
  /// この関数を呼ばないと Luapp と同等になる．
  void
  open_Magus();

};

END_NAMESPACE_MAGUS

#endif // LUAMAGUS_H
