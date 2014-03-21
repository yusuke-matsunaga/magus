
/// @file InputLiteral.cc
/// @brief InputLiteral の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "InputLiteral.h"
#include "TpgNode.h"
#include "TpgPrimitive.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス VectorInputLiteral
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VectorInputLiteral::VectorInputLiteral(const vector<Literal>& vec) :
  mVector(vec)
{
}

// @brief リテラルを返す．
Literal
VectorInputLiteral::operator[](ymuint pos) const
{
  return mVector[pos];
}

// @brief 入力数を返す．
ymuint
VectorInputLiteral::size() const
{
  return mVector.size();
}


//////////////////////////////////////////////////////////////////////
// クラス GvarInputLiteral
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GvarInputLiteral::GvarInputLiteral(const TpgNode* node) :
  mNode(node)
{
}

// @brief リテラルを返す．
Literal
GvarInputLiteral::operator[](ymuint pos) const
{
  const TpgNode* inode = mNode->fanin(pos);
  return Literal(inode->gvar(), false);
}

// @brief 入力数を返す．
ymuint
GvarInputLiteral::size() const
{
  return mNode->fanin_num();
}


//////////////////////////////////////////////////////////////////////
// クラス FvarInputLiteral
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FvarInputLiteral::FvarInputLiteral(const TpgNode* node) :
  mNode(node)
{
}

// @brief リテラルを返す．
Literal
FvarInputLiteral::operator[](ymuint pos) const
{
  const TpgNode* inode = mNode->fanin(pos);
  return Literal(inode->fvar(), false);
}

// @brief 入力数を返す．
ymuint
FvarInputLiteral::size() const
{
  return mNode->fanin_num();
}


//////////////////////////////////////////////////////////////////////
// クラス PrimGvarInputLiteral
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrimGvarInputLiteral::PrimGvarInputLiteral(const TpgPrimitive* prim) :
  mPrim(prim)
{
}

// @brief リテラルを返す．
Literal
PrimGvarInputLiteral::operator[](ymuint pos) const
{
  const TpgPrimitive* iprim = mPrim->fanin(pos);
  return iprim->glit();
}

// @brief 入力数を返す．
ymuint
PrimGvarInputLiteral::size() const
{
  return mPrim->fanin_num();
}


//////////////////////////////////////////////////////////////////////
// クラス PrimFvarInputLiteral
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PrimFvarInputLiteral::PrimFvarInputLiteral(const TpgPrimitive* prim) :
  mPrim(prim)
{
}

// @brief リテラルを返す．
Literal
PrimFvarInputLiteral::operator[](ymuint pos) const
{
  const TpgPrimitive* iprim = mPrim->fanin(pos);
  return iprim->flit();
}

// @brief 入力数を返す．
ymuint
PrimFvarInputLiteral::size() const
{
  return mPrim->fanin_num();
}

END_NAMESPACE_YM_SATPG
