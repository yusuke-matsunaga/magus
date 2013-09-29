
/// @file EufVar.cc
/// @brief EufVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufVar.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufVar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 変数名
EufVar::EufVar(ymuint id,
	       const string& name) :
  EufNode(id),
  mName(name)
{
  mLink = NULL;
}

// @brief デストラクタ
EufVar::~EufVar()
{
}

// @brief 型を得る．
EufNode::tType
EufVar::type() const
{
  return kVar;
}

// @brief 識別子名を得る．
// @note type() が kFunc, kVar の時のみ有効
string
EufVar::id_name() const
{
  return mName;
}

END_NAMESPACE_YM_LLVMEQ
