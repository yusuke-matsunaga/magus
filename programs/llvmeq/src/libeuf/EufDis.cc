
/// @file EufDis.cc
/// @brief EufDis の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufDis.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufDis
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] left, right 左辺と右辺の式
EufDis::EufDis(ymuint id,
	       EufNode* left,
	       EufNode* right) :
  EufBin(id, left, right)
{
}

// @brief デストラクタ
EufDis::~EufDis()
{
}

// @brief 型を得る．
EufNode::tType
EufDis::type() const
{
  return kDis;
}

END_NAMESPACE_YM_LLVMEQ

