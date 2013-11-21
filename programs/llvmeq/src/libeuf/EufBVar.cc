
/// @file EufBVar.cc
/// @brief EufBVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufBVar.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] vid SatSolver 用の変数番号
// @param[in] name 変数名
EufBVar::EufBVar(ymuint id,
		 VarId vid,
		 const string& name) :
  EufVar(id, vid, name)
{
}

// @brief デストラクタ
EufBVar::~EufBVar()
{
}

// @brief 型を得る．
EufNode::tType
EufBVar::type() const
{
  return kBVar;
}

// @brief Boolean 型の時 true を返す．
bool
EufBVar::is_boolean() const
{
  return true;
}

END_NAMESPACE_YM_LLVMEQ
