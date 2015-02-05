#ifndef IRNODE_H
#define IRNODE_H

/// @file IrNode.h
/// @brief IrNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrNode IrNode.h "IrNode.h"
/// @brief IR(intermediate representation) ノードを表す基底クラス
///
/// C の union を C++ 風に実装したようなものなので
/// なんでもありのインターフェイスになっている．
//////////////////////////////////////////////////////////////////////
class IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] type 型
  IrNode(OpCode opcode,
	 const Type* type);

  /// @brief デストラクタ
  virtual
  ~IrNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief opcode を返す．
  OpCode
  opcode() const;

  /// @brief 型を返す．
  const Type*
  type() const;

  /// @brief 第1ソースを返す．
  virtual
  IrNode*
  src1() const;

  /// @brief 第2ソースを返す．
  virtual
  IrNode*
  src2() const;

  /// @brief 第3ソースを返す．
  virtual
  IrNode*
  src3() const;

  /// @brief 整数値を返す．
  ///
  /// kOpIntConst のみ有効
  virtual
  int
  int_val() const;

  /// @brief 実数値を返す．
  ///
  /// kOpFloatConst のみ有効
  virtual
  double
  float_val() const;

  /// @brief 文字列を返す．
  ///
  /// kOpStringConst のみ有効
  virtual
  const char*
  string_val() const;

  /// @brief 変数を返す．
  virtual
  const Var*
  var() const;

  /// @brief 関数本体を返す．
  ///
  /// kOpFuncCall のみ有効
  virtual
  const Function*
  function() const;

  /// @brief 関数の引数の数を得る．
  ///
  /// kOpFuncCall のみ有効
  virtual
  ymuint
  arglist_num() const;

  /// @brief 関数の引数を得る．
  /// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
  ///
  /// kOpFuncCall のみ有効
  virtual
  IrNode*
  arglist_elem(ymuint pos) const;

  /// @brief ジャンプ先のノードを得る．
  ///
  /// kOpJump, kOpBranchXXX のみ有効
  virtual
  IrNode*
  jump_addr() const;

  /// @brief 分岐条件
  virtual
  IrNode*
  branch_cond() const;

  /// @brief 返り値
  virtual
  IrNode*
  return_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペコード
  OpCode mOpCode;

  // 型
  const Type* mType;

};

END_NAMESPACE_YM_YMSL

#endif // IRNODE_H
