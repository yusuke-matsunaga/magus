
/// @file IrNode.cc
/// @brief IrNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] irtype IR型
// @param[in] type 型
IrNode::IrNode(IrType irtype,
	       const Type* type) :
  mIrType(irtype),
  mType(type)
{
}

// @brief デストラクタ
IrNode::~IrNode()
{
}

// @brief ノードの型(IR型)を返す．
IrNode::IrType
IrNode::node_type() const
{
  return mIrType;
}

// @brief 値の型を返す．
const Type*
IrNode::value_type() const
{
  return mType;
}

// @brief 番号を返す．
ymuint
IrNode::id() const
{
  return mId;
}

// @brief 番号をセットする．
// @param[in] id 番号
void
IrNode::set_id(ymuint id)
{
  mId = id;
}

// @brief オペコードを返す．
//
// 演算子のみ有効
OpCode
IrNode::opcode() const
{
  ASSERT_NOT_REACHED;
  return kOpCastBoolean;
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
IrNode::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num() )
//
// 演算子のみ有効
IrNode*
IrNode::operand(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief ロード/ストア対象のアドレスを得る．
//
// kLoad, kStore, kInc, kDec のみ有効
IrHandle*
IrNode::address() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 書き込む値を返す．
//
// kStore のみ有効
IrNode*
IrNode::store_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数インデックスを設定する．
// @param[in] index 関数インデックス
//
// kFuncCall のみ有効
void
IrNode::set_function_index(ymuint index)
{
  ASSERT_NOT_REACHED;
}

// @brief 関数インデックスを返す．
//
// kFuncCall のみ有効
ymuint
IrNode::function_index() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 関数の引数の数を得る．
//
// kFuncCall のみ有効
ymuint
IrNode::arglist_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 関数の引数を得る．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
IrNode*
IrNode::arglist_elem(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief ジャンプ先のノードを得る．
//
// kJump, kBranchXXX のみ有効
IrNode*
IrNode::jump_addr() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 分岐条件
IrNode*
IrNode::branch_cond() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 返り値
IrNode*
IrNode::return_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 定義済みの時に true を返す．
//
// kLabel のみ意味を持つ．
bool
IrNode::is_defined() const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief 定義済みにする．
//
// kLabel のみ意味を持つ．
void
IrNode::set_defined()
{
  ASSERT_NOT_REACHED;
}


//////////////////////////////////////////////////////////////////////
// その他
//////////////////////////////////////////////////////////////////////

// @brief IrType を出力する．
// @param[in] s 出力先のストリーム
// @param[in] op オペオード
ostream&
operator<<(ostream& s,
	   IrNode::IrType op)
{
  switch ( op ) {
  case IrNode::kUniOp:         s << "UniOp"; break;
  case IrNode::kBinOp:         s << "BinOp"; break;
  case IrNode::kTriOp:         s << "TriOp"; break;
  case IrNode::kLoad:          s << "Load"; break;
  case IrNode::kStore:         s << "Store"; break;
  case IrNode::kFuncCall:      s << "FuncCall"; break;
  case IrNode::kReturn:        s << "Return"; break;
  case IrNode::kJump:          s << "Jump"; break;
  case IrNode::kBranchTrue:    s << "BranchTrue"; break;
  case IrNode::kBranchFalse:   s << "BranchFalse"; break;
  case IrNode::kLabel:         s << "Label"; break;
  case IrNode::kHalt:          s << "Halt"; break;
  default: ASSERT_NOT_REACHED; break;
  }

  return s;
}

END_NAMESPACE_YM_YMSL
