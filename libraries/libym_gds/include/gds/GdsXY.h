#ifndef GDS_GDSXY_H
#define GDS_GDSXY_H

/// @file gds/GdsXY.h
/// @brief GdsXY のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsXY GdsXY.h "gds/GdsXY.h"
/// @brief 点列を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsXY
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  GdsXY();

  /// @brief デストラクタ
  ~GdsXY();


public:

  /// @brief 要素数を返す．
  ymuint
  num() const;

  /// @brief pos 番めの X 座標を返す．
  /// @param[in] pos 位置 ( 0 <= pos < num() )
  ymint32
  x(ymuint pos) const;

  /// @brief pos 番めの Y 座標を返す．
  /// @param[in] pos 位置 ( 0 <= pos < num() )
  ymint32
  y(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mNum;

  // データの配列
  ymint32 mData[1];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
GdsXY::GdsXY()
{
}

// @brief デストラクタ
inline
GdsXY::~GdsXY()
{
}

// @brief 要素数を返す．
inline
ymuint
GdsXY::num() const
{
  return mNum;
}

// @brief pos 番めの X 座標を返す．
// @param[in] pos 位置 ( 0 <= pos < num() )
inline
ymint32
GdsXY::x(ymuint pos) const
{
  return mData[pos * 2 + 0];
}

// @brief pos 番めの Y 座標を返す．
// @param[in] pos 位置 ( 0 <= pos < num() )
inline
ymint32
GdsXY::y(ymuint pos) const
{
  return mData[pos * 2 + 1];
}

END_NAMESPACE_YM_GDS

#endif // GDS_GDSXY_H
