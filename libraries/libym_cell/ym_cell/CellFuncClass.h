#ifndef YM_CELL_CELLFUNCCLASS_H
#define YM_CELL_CELLFUNCCLASS_H

/// @file libym_cell/CellFuncClass.h
/// @brief CellFuncClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellFuncClass CellFuncClass.h "ym_cell/CellFuncClass.h"
/// @brief 代表関数を表すクラス
//////////////////////////////////////////////////////////////////////
class CellFuncClass
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CellFuncClass();

  /// @brief デストラクタ
  ~CellFuncClass();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief NPN同値類の数を返す．
  ymuint
  func_num() const;

  /// @brief 関数番号を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
  ymuint
  func_id(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数の数
  ymuint32 mFuncNum;

  // 関数番号の配列
  ymuint32* mFuncIdList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
CellFuncClass::CellFuncClass() :
  mFuncNum(0U),
  mFuncIdList(NULL)
{
}

// @brief デストラクタ
inline
CellFuncClass::~CellFuncClass()
{
  // mFuncIdList は CellMgr が管理している．
}

// @brief NPN同値類の数を返す．
inline
ymuint
CellFuncClass::func_num() const
{
  return mFuncNum;
}

// @brief 関数番号を返す．
// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
inline
ymuint
CellFuncClass::func_id(ymuint pos) const
{
  return mFuncIdList[pos];
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLFUNCCLASS_H
