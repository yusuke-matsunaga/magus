
/// @file LitMap.cc
/// @brief LitMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "VectLitMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス VectLitMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VectLitMap::VectLitMap(const vector<VarId>& ivars,
		       VarId ovar) :
  mIvars(ivars),
  mOvar(ovar)
{
}

// @brief 入力数を返す．
ymuint
VectLitMap::input_size() const
{
  return mIvars.size();
}

// @brief 入力のリテラルを返す．
Literal
VectLitMap::input(ymuint pos) const
{
  return Literal(mIvars[pos], false);
}

// @brief 出力のリテラルを返す．
Literal
VectLitMap::output() const
{
  return Literal(mOvar, false);
}

END_NAMESPACE_YM_SATPG
