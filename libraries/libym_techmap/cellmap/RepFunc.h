#ifndef LIBYM_TECHMAP_CELLMAP_REPFUNC_H
#define LIBYM_TECHMAP_CELLMAP_REPFUNC_H

/// @file libym_techmap/cellmap/RepFunc.h
/// @brief RepFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/cellmap_nsdef.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class RepFunc RepFunc.h "RepFunc.h"
/// @brief 代表関数を表すクラス
//////////////////////////////////////////////////////////////////////
class RepFunc
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  RepFunc();

  /// @brief デストラクタ
  ~RepFunc();


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
RepFunc::RepFunc() :
  mFuncNum(0U),
  mFuncIdList(NULL)
{
}

// @brief デストラクタ
inline
RepFunc::~RepFunc()
{
  // mFuncIdList は CellMgr が管理している．
}

// @brief NPN同値類の数を返す．
inline
ymuint
RepFunc::func_num() const
{
  return mFuncNum;
}

// @brief 関数番号を返す．
// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
inline
ymuint
RepFunc::func_id(ymuint pos) const
{
  return mFuncIdList[pos];
}

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_REPFUNC_H
