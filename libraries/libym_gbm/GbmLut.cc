
/// @file GbmLut.cc
/// @brief GbmLut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GbmLut.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
// クラス GbmLut
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] vid 変数番号
// @param[in] inputs 入力のハンドルのリスト
// @param[in] tv_list 真理値ベクタの変数番号のリスト
GbmLut::GbmLut(ymuint id,
	       VarId vid,
	       const vector<GbmNodeHandle>& inputs,
	       const vector<VarId>& tv_list) :
  GbmNodeImpl(id, vid),
  mFanins(inputs),
  mTvList(tv_list)
{
}

// @brief デストラクタ
GbmLut::~GbmLut()
{
}

// @brief LUTノードの時 true を返す．
bool
GbmLut::is_lut() const
{
  return true;
}

// @brief ファンイン数を返す．
// @note 外部入力ノードの場合は常に0
// @note AND ノードの場合は常に2
ymuint
GbmLut::fanin_num() const
{
  return mFanins.size();
}

// @brief ファンインのハンドルを返す．
// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
GbmNodeHandle
GbmLut::fanin(ymuint pos) const
{
  assert_cond( pos < fanin_num(), __FILE__, __LINE__);
  return mFanins[pos];
}

END_NAMESPACE_YM_GBM
