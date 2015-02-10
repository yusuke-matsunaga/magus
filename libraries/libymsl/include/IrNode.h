#ifndef IRNODE_H
#define IRNODE_H

/// @file IrNode.h
/// @brief IrNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "OpCode.h"


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
  //////////////////////////////////////////////////////////////////////
  /// @brief IrNode の種類
  //////////////////////////////////////////////////////////////////////
  enum IrType {
    // 定数
    kTrue,
    kFalse,
    kIntConst,
    kFloatConst,
    kStringConst,
    // 単項演算
    kUniOp,
    // 二項演算
    kBinOp,
    // 三項演算
    kTriOp,
    // ロード/ストア
    kLoad,
    kStore,
    // 演算付き代入
    kInplaceUniOp,
    kInplaceBinOp,
    // 関数呼び出し/復帰
    kFuncCall,
    kReturn,
    // ジャンプ
    kJump,
    kBranchTrue,
    kBranchFalse,
    // 特殊
    kLabel,
    // 停止
    kHalt
  };

public:

  /// @brief コンストラクタ
  /// @param[in] irtype IRタイプ
  /// @param[in] type 型
  IrNode(IrType irtype,
	 const Type* type);

  /// @brief デストラクタ
  virtual
  ~IrNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの型を返す．
  IrType
  node_type() const;

  /// @brief 値の型を返す．
  ///
  /// 値を返すタイプのみ意味を持つ．
  /// それ以外のノードでは NULL を返す．
  const Type*
  value_type() const;

  /// @brief 番号を返す．
  ymuint
  id() const;

  /// @brief 番号をセットする．
  /// @param[in] id 番号
  void
  set_id(ymuint id);

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const = 0;

  /// @brief オペコードを返す．
  ///
  /// 演算子のみ有効
  virtual
  OpCode
  opcode() const;

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
  /// kIntConst のみ有効
  virtual
  int
  int_val() const;

  /// @brief 実数値を返す．
  ///
  /// kFloatConst のみ有効
  virtual
  double
  float_val() const;

  /// @brief 文字列を返す．
  ///
  /// kStringConst のみ有効
  virtual
  const char*
  string_val() const;

  /// @brief ロード/ストア対象のアドレスを得る．
  ///
  /// kLoad, kStore, kInplaceUniOp, kInplaceBinOp のみ有効
  virtual
  IrHandle*
  address() const;

  /// @brief 書き込む値を返す．
  ///
  /// kStore のみ有効
  virtual
  IrNode*
  store_val() const;

  /// @brief 関数アドレスを返す．
  ///
  /// kFuncCall のみ有効
  virtual
  IrHandle*
  func_addr() const;

  /// @brief 関数の引数の数を得る．
  ///
  /// kFuncCall のみ有効
  virtual
  ymuint
  arglist_num() const;

  /// @brief 関数の引数を得る．
  /// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
  ///
  /// kFuncCall のみ有効
  virtual
  IrNode*
  arglist_elem(ymuint pos) const;

  /// @brief ジャンプ先のノードを得る．
  ///
  /// kJump, kBranchXXX のみ有効
  virtual
  IrNode*
  jump_addr() const;

  /// @brief 分岐条件
  ///
  /// kBranchXXX のみ有効
  virtual
  IrNode*
  branch_cond() const;

  /// @brief 返り値
  ///
  /// kReturn のみ有効
  virtual
  IrNode*
  return_val() const;

  /// @brief 定義済みの時に true を返す．
  ///
  /// kLabel のみ意味を持つ．
  virtual
  bool
  is_defined() const;

  /// @brief 定義済みにする．
  ///
  /// kLabel のみ意味を持つ．
  virtual
  void
  set_defined();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペコード
  IrType mIrType;

  // 型
  const Type* mType;

  // 番号
  ymuint mId;

};

/// @brief IrType を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] irtype IR型
ostream&
operator<<(ostream& s,
	   IrNode::IrType irtype);

END_NAMESPACE_YM_YMSL

#endif // IRNODE_H
