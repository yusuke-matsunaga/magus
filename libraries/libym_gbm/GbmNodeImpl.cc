
/// @file GbmNodeImpl.cc
/// @brief GbmNodeImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNodeImpl.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
// クラス GbmNodeImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] vid 変数番号
GbmNodeImpl::GbmNodeImpl(ymuint id,
			 VarId vid) :
  mId(id),
  mVarId(vid)
{
}

// @brief デストラクタ
GbmNodeImpl::~GbmNodeImpl()
{
}

// @brief ID番号を返す．
ymuint
GbmNodeImpl::id() const
{
  return mId;
}

// @brief SATソルバ用の変数番号を返す．
VarId
GbmNodeImpl::var_id() const
{
  return mVarId;
}

// @brief 外部入力ノードの時 true を返す．
bool
GbmNodeImpl::is_input() const
{
  return false;
}

// @brief ANDノードの時 true を返す．
bool
GbmNodeImpl::is_and() const
{
  return false;
}

// @brief LUTノードの時 true を返す．
bool
GbmNodeImpl::is_lut() const
{
  return false;
}

END_NAMESPACE_YM_GBM
