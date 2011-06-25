#ifndef LIBYM_TECHMAP_CELLMAP_FFGROUP_H
#define LIBYM_TECHMAP_CELLMAP_FFGROUP_H

/// @file libym_techmap/cellmap/FFGroup.h
/// @brief FFGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellGroup.h"
#include "FFPosArray.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class FFGroup FFGroup.h "FFGroup.h"
/// @brief FF のグループを表すクラス
//////////////////////////////////////////////////////////////////////
class FFGroup :
  public CellGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  FFGroup();

  /// @brief デストラクタ
  ~FFGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピンの割り当て情報を返す．
  const FFPosArray&
  pos_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ピン番号の情報
  FFPosArray mPosArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
FFGroup::FFGroup()
{
}

// @brief デストラクタ
inline
FFGroup::~FFGroup()
{
}

// @brief ピンの割り当て情報を返す．
inline
const FFPosArray&
FFGroup::pos_array() const
{
  return mPosArray;
}

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_FFGROUP_H
