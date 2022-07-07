#ifndef CUBE_H
#define CUBE_H

/// @file Cube.h
/// @brief Cube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "trusyn.h"


BEGIN_NAMESPACE_TRUSYN

//////////////////////////////////////////////////////////////////////
/// @class Cube Cube.h "Cube.h"
/// @brief キューブを表すクラス
///
/// 高々20入力以下なので64bitの1ワードで表すことができる．
/// ただし，入力数やマスクなどの情報は他の場所で保持する必要がある．
//////////////////////////////////////////////////////////////////////
class Cube
{
public:

  enum Pat {
    POSI = 1,
    NEGA = 2,
    NONE = 3
  };

public:

  /// @brief コンストラクタ
  Cube() = default;

  /// @brief デストラクタ
  ~Cube() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  Pat
  get(
    SizeType pos ///< [in] 位置
  )
  {
    return static_cast<Pat>((mBody >> (pos * 2)) & 3);
  }

  /// @brief 値を設定する．
  void
  set(
    SizeType pos, ///< [in] 位置
    Pat pat       ///< [in] 値
  )
  {
    mBody &= ~(3 << (pos * 2));
    mBody |= static_cast<ymuint64>(pat) << (pos * 2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  ymuint64 mBody{0UL};

};

END_NAMESPACE_TRUSYN

#endif // CUBE_H
