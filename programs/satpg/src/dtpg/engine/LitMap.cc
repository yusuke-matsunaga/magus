
/// @file LitMap.cc
/// @brief LitMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "LitMap.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス GvarLitMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GvarLitMap::GvarLitMap(const TpgNode* node) :
  mNode(node)
{
}

// @brief 入力のリテラルを返す．
Literal
GvarLitMap::input(ymuint pos) const
{
  const TpgNode* inode = mNode->fanin(pos);
  return Literal(inode->gvar(), false);
}

// @brief 入力数を返す．
ymuint
GvarLitMap::input_size() const
{
  return mNode->fanin_num();
}


//////////////////////////////////////////////////////////////////////
// クラス FvarLitMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FvarLitMap::FvarLitMap(const TpgNode* node) :
  mNode(node)
{
}

// @brief 入力のリテラルを返す．
Literal
FvarLitMap::input(ymuint pos) const
{
  const TpgNode* inode = mNode->fanin(pos);
  return Literal(inode->fvar(), false);
}

// @brief 入力数を返す．
ymuint
FvarLitMap::input_size() const
{
  return mNode->fanin_num();
}


//////////////////////////////////////////////////////////////////////
// クラス VectLitMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VectLitMap::VectLitMap(const vector<VarId>& ivars) :
  mIvars(ivars)
{
}

// @brief 入力のリテラルを返す．
Literal
VectLitMap::input(ymuint pos) const
{
  return Literal(mIvars[pos], false);
}

// @brief 入力数を返す．
ymuint
VectLitMap::input_size() const
{
  return mIvars.size();
}

END_NAMESPACE_YM_SATPG
