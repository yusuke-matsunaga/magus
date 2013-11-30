
/// @file GbmAnd.cc
/// @brief GbmAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GbmAnd.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
// クラス GbmAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] vid 変数番号
// @param[in] input0 ファンイン0のハンドル
// @param[in] input1 ファンイン1のハンドル
GbmAnd::GbmAnd(ymuint id,
	       VarId vid,
	       GbmNodeHandle input0,
	       GbmNodeHandle input1) :
  GbmNodeImpl(id, vid),
  mFanin0(input0),
  mFanin1(input1)
{
}

// @brief デストラクタ
GbmAnd::~GbmAnd()
{
}

// @brief ANDノードの時 true を返す．
bool
GbmAnd::is_and() const
{
  return true;
}

// @brief ファンイン数を返す．
// @note 外部入力ノードの場合は常に0
// @note AND ノードの場合は常に2
ymuint
GbmAnd::fanin_num() const
{
  return 2;
}

// @brief ファンインのハンドルを返す．
// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
GbmNodeHandle
GbmAnd::fanin(ymuint pos) const
{
  if ( pos == 0 ) {
    return mFanin0;
  }
  if ( pos == 1 ) {
    return mFanin1;
  }
  assert_not_reached(__FILE__, __LINE__);
  return GbmNodeHandle();
}

END_NAMESPACE_YM_GBM
