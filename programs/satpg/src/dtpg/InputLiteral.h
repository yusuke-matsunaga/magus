#ifndef INPUTLITERAL_H
#define INPUTLITERAL_H

/// @file InputLiteral.h
/// @brief InputLiteral のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "logic/Literal.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class InputLiteral InputLiteral.h "InputLiter.h"
/// @brief make_gate_cnf で用いられるファンクター
//////////////////////////////////////////////////////////////////////
struct InputLiteral
{

  /// @brief リテラルを返す．
  virtual
  Literal
  operator[](ymuint pos) const = 0;

  /// @brief 入力数を返す．
  virtual
  ymuint
  size() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VectorInputLiteral InputLiteral.h "InputLiter.h"
/// @brief vector<> を用いた InputLiteral
//////////////////////////////////////////////////////////////////////
class VectorInputLiteral :
  public InputLiteral
{
public:

  /// @brief コンストラクタ
  VectorInputLiteral(const vector<Literal>& vec);

  /// @brief リテラルを返す．
  virtual
  Literal
  operator[](ymuint pos) const;

  /// @brief 入力数を返す．
  virtual
  ymuint
  size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベクタ
  const vector<Literal>& mVector;

};


//////////////////////////////////////////////////////////////////////
/// @class GvarInputLiteral InputLiteral.h "InputLiter.h"
/// @brief TpgNode->gvar() を用いた InputLiteral
//////////////////////////////////////////////////////////////////////
class GvarInputLiteral :
  public InputLiteral
{
public:

  /// @brief コンストラクタ
  GvarInputLiteral(const TpgNode* node);

  /// @brief リテラルを返す．
  virtual
  Literal
  operator[](ymuint pos) const;

  /// @brief 入力数を返す．
  virtual
  ymuint
  size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード
  const TpgNode* mNode;

};


//////////////////////////////////////////////////////////////////////
/// @class FvarInputLiteral InputLiteral.h "InputLiter.h"
/// @brief TpgNode->fvar() を用いた InputLiteral
//////////////////////////////////////////////////////////////////////
class FvarInputLiteral :
  public InputLiteral
{
public:

  /// @brief コンストラクタ
  FvarInputLiteral(const TpgNode* node);

  /// @brief リテラルを返す．
  virtual
  Literal
  operator[](ymuint pos) const;

  /// @brief 入力数を返す．
  virtual
  ymuint
  size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード
  const TpgNode* mNode;

};


//////////////////////////////////////////////////////////////////////
/// @class PrimGvarInputLiteral InputLiteral.h "InputLiter.h"
/// @brief TpgPrimitive->gvar() を用いた InputLiteral
//////////////////////////////////////////////////////////////////////
class PrimGvarInputLiteral :
  public InputLiteral
{
public:

  /// @brief コンストラクタ
  PrimGvarInputLiteral(const TpgPrimitive* prim);

  /// @brief リテラルを返す．
  virtual
  Literal
  operator[](ymuint pos) const;

  /// @brief 入力数を返す．
  virtual
  ymuint
  size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // プリミティブ
  const TpgPrimitive* mPrim;

};


//////////////////////////////////////////////////////////////////////
/// @class PrimFvarInputLiteral InputLiteral.h "InputLiter.h"
/// @brief TpgPrimitive->fvar() を用いた InputLiteral
//////////////////////////////////////////////////////////////////////
class PrimFvarInputLiteral :
  public InputLiteral
{
public:

  /// @brief コンストラクタ
  PrimFvarInputLiteral(const TpgPrimitive* prim);

  /// @brief リテラルを返す．
  virtual
  Literal
  operator[](ymuint pos) const;

  /// @brief 入力数を返す．
  virtual
  ymuint
  size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // プリミティブ
  const TpgPrimitive* mPrim;

};

END_NAMESPACE_YM_SATPG

#endif // INPUTLITERAL_H