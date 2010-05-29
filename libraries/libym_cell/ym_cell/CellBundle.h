#ifndef YM_CELL_CELLBUNDLE_H
#define YM_CELL_CELLBUNDLE_H

/// @file　ym_cell/CellBundle.h
/// @brief CellBundle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellBundle.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellBundle CellBundle.h <ym_cell/CellBundle.h>
/// @brief バンドルを表すクラス
//////////////////////////////////////////////////////////////////////
class CellBundle
{
  friend class CellManip;

private:

  /// @brief コンストラクタ
  CellBundle(ShString name,
	     ymuint32 n_pin);

  /// @brief デストラクタ
  ~CellBundle();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  ShString
  name() const;

  /// @brief ピン数の取得
  ymuint32
  n_pins() const;

  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < n_pins() )
  const CellPin*
  pin(ymuint32 pos) const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 名前
  ShString mName;
  
  // ピンのリスト
  vector<CellPin*> mPinList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 名前の取得
inline
ShString
CellBundle::name() const
{
  return mName;
}

// @brief ピン数の取得
inline
ymuint32
CellBundle::n_pins() const
{
  return mPinList.size();
}

// @brief ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < n_pins() )
inline
const CellPin*
CellBundle::pin(ymuint32 pos) const
{
  return mPinList[pos];
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLBUNDLE_H
