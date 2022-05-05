#ifndef LUAMAGUS_H
#define LUAMAGUS_H

/// @file LuaMagus.h
/// @brief LuaMagus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "Luapp.h"
#include "ym/bnet.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class LuaMagus LuaMagus.h "LuaMagus.h"
/// @brief Magus 用の Luapp 拡張
//////////////////////////////////////////////////////////////////////
class LuaMagus :
  public Luapp
{
public:

  /// @brief 空のコンストラクタ
  LuaMagus() = default;

  /// @brief 通常のコンストラクタ
  LuaMagus(
    lua_Alloc f, ///< [in] アロケーター
    void* ud      ///< [in] ユーザーデータ
  ) : Luapp{f, ud}
  {
  }

  /// @brief すでに生成済みのインタプリタを用いるコンストラクタ
  LuaMagus(
    lua_State* L ///< [in] lua インタープリタ
  ) : Luapp{L}
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

  /// @brief 対象が BnNetwork の時 true を返す．
  bool
  is_bnet(
    int idx ///< [in] スタック上のインデックス
  );

  /// @brief 対象を BnNetwork として取り出す．
  ///
  /// BnNetwork でない時は nullptr を返す．
  BnNetwork*
  to_bnet(
    int idx ///< [in] スタック上のインデックス
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNetwork 関係の初期化を行う．
  void
  init_Bnet();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_MAGUS

#endif // LUAMAGUS_H