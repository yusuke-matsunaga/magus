
/// @file VarSplitter.cc
/// @brief VarSplitter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "VarSplitter.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス VarSplitter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] varid 変数番号
VarSplitter::VarSplitter(ymuint32 varid) :
  mVarId(varid)
{
}

// @brief デストラクタ
VarSplitter::~VarSplitter()
{
}

// @brief 分類数を返す．
ymuint
VarSplitter::split_num() const
{
  return 2;
}

// @brief ベクタを分類する．
// @param[in] vect 対象のベクタ
ymuint
VarSplitter::operator()(const RegVect* vect) const
{
  return vect->val(mVarId);
}

END_NAMESPACE_YM_IGF
