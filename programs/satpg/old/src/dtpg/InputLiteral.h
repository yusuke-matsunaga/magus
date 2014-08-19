#ifndef INPUTLITERAL_H
#define INPUTLITERAL_H

/// @file atpg/src/dtpg/InputLiteral.h
/// @brief InputLiteral のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "ym_YmLogic/Literal.h"
#if 0
#include "DtpgNetwork.h"
#include "DtpgNode.h"
#include "DtpgPrimitive.h"
#include "DtpgFFR.h"
#include "DtpgFault.h"
#include "DtpgOperator.h"
#include "ym_YmLogic/SatSolver.h"
#include "ym_YmLogic/SatStats.h"
#endif

BEGIN_NAMESPACE_YM_SATPG_DTPG

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
/// @brief DtpgNode->gvar() を用いた InputLiteral
//////////////////////////////////////////////////////////////////////
class GvarInputLiteral :
  public InputLiteral
{
public:

  /// @brief コンストラクタ
  GvarInputLiteral(DtpgNode* node);

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
  DtpgNode* mNode;

};


//////////////////////////////////////////////////////////////////////
/// @class FvarInputLiteral InputLiteral.h "InputLiter.h"
/// @brief DtpgNode->fvar() を用いた InputLiteral
//////////////////////////////////////////////////////////////////////
class FvarInputLiteral :
  public InputLiteral
{
public:

  /// @brief コンストラクタ
  FvarInputLiteral(DtpgNode* node);

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
  DtpgNode* mNode;

};


//////////////////////////////////////////////////////////////////////
/// @class PrimGvarInputLiteral InputLiteral.h "InputLiter.h"
/// @brief DtpgPrimitive->gvar() を用いた InputLiteral
//////////////////////////////////////////////////////////////////////
class PrimGvarInputLiteral :
  public InputLiteral
{
public:

  /// @brief コンストラクタ
  PrimGvarInputLiteral(DtpgPrimitive* prim);

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
  DtpgPrimitive* mPrim;

};


//////////////////////////////////////////////////////////////////////
/// @class PrimFvarInputLiteral InputLiteral.h "InputLiter.h"
/// @brief DtpgPrimitive->fvar() を用いた InputLiteral
//////////////////////////////////////////////////////////////////////
class PrimFvarInputLiteral :
  public InputLiteral
{
public:

  /// @brief コンストラクタ
  PrimFvarInputLiteral(DtpgPrimitive* prim);

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
  DtpgPrimitive* mPrim;

};

END_NAMESPACE_YM_SATPG_DTPG

#endif // INPUTLITERAL_H
