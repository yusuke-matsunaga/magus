
/// @file EiTernaryOp.cc
/// @brief EiTernaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiTernaryOp.h"

#include "verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 3項演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] op_type 演算子のタイプ
// @param[in] opr0 オペランド
// @param[in] opr1 オペランド
// @param[in] opr2 オペランド
ElbExpr*
EiFactory::new_TernaryOp(const PtExpr* pt_expr,
			 tVlOpType op_type,
			 ElbExpr* opr0,
			 ElbExpr* opr1,
			 ElbExpr* opr2)
{
  ElbExpr* expr = NULL;
  void* p;
  switch ( op_type ) {
  case kVlConditionOp:
    p = mAlloc.get_memory(sizeof(EiConditionOp));
    expr = new (p) EiConditionOp(pt_expr, opr0, opr1, opr2);
    break;

  case kVlMinTypMaxOp:
    p = mAlloc.get_memory(sizeof(EiMinTypMaxOp));
    expr = new (p) EiMinTypMaxOp(pt_expr, opr0, opr1, opr2);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTernaryOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @param[in] opr3 オペランド3
EiTernaryOp::EiTernaryOp(const PtExpr* pt_expr,
			 ElbExpr* opr1,
			 ElbExpr* opr2,
			 ElbExpr* opr3) :
  EiOperation(pt_expr)
{
  mOpr[0] = opr1;
  mOpr[1] = opr2;
  mOpr[2] = opr3;
}

// @brief デストラクタ
EiTernaryOp::~EiTernaryOp()
{
}

// @brief 定数の時 true を返す．
// @note オペランドが定数ならこの式も定数となる．
bool
EiTernaryOp::is_const() const
{
  return operand1()->is_const() &&
    operand2()->is_const() && operand3()->is_const();

}

// @brief オペランド数を返す．
ymuint
EiTernaryOp::operand_num() const
{
  return 3;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
ElbExpr*
EiTernaryOp::_operand(ymuint pos) const
{
  return mOpr[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス EiConditionOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @param[in] opr3 オペランド3
EiConditionOp::EiConditionOp(const PtExpr* pt_expr,
			     ElbExpr* opr1,
			     ElbExpr* opr2,
			     ElbExpr* opr3) :
  EiTernaryOp(pt_expr, opr1, opr2, opr3)
{
  // 三項演算子の場合は第1オペランドが self determined で
  // 結果は第2オペランドと第3オペランドから決まる．

  operand1()->set_selfsize();

  VlValueType type2 = operand2()->value_type();
  VlValueType type3 = operand3()->value_type();
  mType = calc_type(type2, type3);
}

// @brief デストラクタ
EiConditionOp::~EiConditionOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiConditionOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiConditionOp::_set_reqsize(const VlValueType& type)
{
  mType = update_size(mType, type);

  // 第1オペランドは不変

  // 第2, 第3オペランドは type をそのまま伝える．
  operand2()->set_reqsize(mType);
  operand3()->set_reqsize(mType);
}


//////////////////////////////////////////////////////////////////////
// クラス EiMinTypMaxOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド1
// @param[in] opr2 オペランド2
// @param[in] opr3 オペランド3
EiMinTypMaxOp::EiMinTypMaxOp(const PtExpr* pt_expr,
			     ElbExpr* opr1,
			     ElbExpr* opr2,
			     ElbExpr* opr3) :
  EiTernaryOp(pt_expr, opr1, opr2, opr3)
{
  // とりあえず真ん中の式を使う．
  mType = opr2->value_type();
}

// @brief デストラクタ
EiMinTypMaxOp::~EiMinTypMaxOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiMinTypMaxOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiMinTypMaxOp::_set_reqsize(const VlValueType& type)
{
  mType = update_size(mType, type);

  // 3オペランドには type をそのまま伝える．
  operand1()->set_reqsize(mType);
  operand2()->set_reqsize(mType);
  operand3()->set_reqsize(mType);
}

END_NAMESPACE_YM_VERILOG
