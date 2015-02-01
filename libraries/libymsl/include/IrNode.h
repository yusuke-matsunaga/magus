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
//////////////////////////////////////////////////////////////////////
class IrNode
{
public:

  /// @brief デストラクタ
  virtual
  ~IrNode() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief opcode を返す．
  virtual
  OpCode
  opcode() const = 0;

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

  /// @brief ジャンプ先のノードを設定する．
  /// @param[in] node ジャンプ先のノード
  ///
  /// kOpJump, kOpBranchXXX のみ有効
  virtual
  void
  set_addr(IrNode* node);

};

END_NAMESPACE_YM_YMSL

#endif // IRNODE_H
