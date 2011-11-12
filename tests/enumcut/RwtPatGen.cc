
/// @file RwtPatGen.cc
/// @brief RwtPatGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RwtPatGen.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス RwtPatGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RwtPatGen::RwtPatGen()
{
  mInputNum = 0;
}

// @brief デストラクタ
RwtPatGen::~RwtPatGen()
{
}

// @brief 入力数を得る．
ymuint
RwtPatGen::input_num() const
{
  return mInputNum;
}

// @brief ノード数を得る．
ymuint
RwtPatGen::node_num() const
{
  return mNodeArray.size();
}

// @brief ノードを得る．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
const RwtNode*
RwtPatGen::node(ymuint id) const
{
  assert_cond( id < node_num(), __FILE__, __LINE__);
  return mNodeArray[id];
}

// @brief 登録されている関数の数を返す．
ymuint
RwtPatGen::func_num() const
{
  return mFuncList.size();
}

// @brief 関数を得る．
// @param[in] pos 関数番号 ( 0 <= pos < func_num() )
TvFunc
RwtPatGen::func(ymuint pos) const
{
  assert_cond( pos < func_num(), __FILE__, __LINE__);
  return mFuncList[pos].mFunc;
}

// @brief 関数に対応するパタンのリストを得る．
// @param[in] pos 関数番号 ( 0 <= pos < func_num() )
const vector<RwtPat*>&
RwtPatGen::pat_list(ymuint pos) const
{
  assert_cond( pos < func_num(), __FILE__, __LINE__);
  return mFuncList[pos].mPatList;
}

END_NAMESPACE_YM_NETWORKS
