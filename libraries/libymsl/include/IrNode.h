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

  /// @brief 番号を返す．
  ymuint
  id() const;

  /// @brief 番号をセットする．
  /// @param[in] id 番号
  void
  set_id(ymuint id);

  /// @brief オペランド数を返す．
  ///
  /// 演算子のみ有効
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置 ( 0 <= pos < operand_num() )
  ///
  /// 演算子のみ有効
  virtual
  IrNode*
  operand(ymuint pos) const;

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
  ///
  /// kOpLoad, kOpStore, kOpMemberLoad, kOpMemberStore のみ有効
  virtual
  const Var*
  var() const;

  /// @brief 書き込む値を返す．
  ///
  /// kOpStore, kOpArrayStore, kOpMemberStore のみ有効
  virtual
  IrNode*
  store_val() const;

  /// @brief 配列本体の式を返す．
  ///
  /// kOpArrayLoad, kOpArrayStore のみ有効
  virtual
  IrNode*
  array_expr() const;

  /// @brief 配列のインデックスを返す．
  ///
  /// kOpArrayLoad, kOpArrayStore のみ有効
  virtual
  IrNode*
  array_index() const;

  /// @brief オブジェクトを指す式を返す．
  ///
  /// kOpMemberLoad, kOpMemberStore のみ有効
  virtual
  IrNode*
  obj_expr() const;

  /// @brief メンバのインデックスを返す．
  ///
  /// kOpMemberLoad, kOpMemberStore のみ有効
  virtual
  ymuint
  member_index() const;

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
  ///
  /// kOpBranchXXX のみ有効
  virtual
  IrNode*
  branch_cond() const;

  /// @brief 返り値
  ///
  /// kOpReturn のみ有効
  virtual
  IrNode*
  return_val() const;

  /// @brief 定義済みの時に true を返す．
  ///
  /// kOpLabel のみ意味を持つ．
  virtual
  bool
  is_defined() const;

  /// @brief 定義済みにする．
  ///
  /// kOpLabel のみ意味を持つ．
  virtual
  void
  set_defined();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペコード
  OpCode mOpCode;

  // 型
  const Type* mType;

  // 番号
  ymuint mId;

};

END_NAMESPACE_YM_YMSL

#endif // IRNODE_H
