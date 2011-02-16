
/// @file libym_verilog/elb_impl/EiUnaryOp.cc
/// @brief EiUnaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiUnaryOp.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiUnaryOp.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 単項演算子を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] op_type 演算子のタイプ
// @param[in] opr1 オペランド
ElbExpr*
EiFactory::new_UnaryOp(const PtExpr* pt_expr,
		       tVpiOpType op_type,
		       ElbExpr* opr1)
{
  ElbExpr* expr = NULL;
  void* p;
  switch ( op_type ) {
  case kVpiPosedgeOp:
  case kVpiNegedgeOp:
    p = mAlloc.get_memory(sizeof(EiEventEdgeOp));
    expr = new (p) EiEventEdgeOp(pt_expr, opr1);
    break;

  case kVpiBitNegOp:
    p = mAlloc.get_memory(sizeof(EiBitNegOp));
    expr = new (p) EiBitNegOp(pt_expr, opr1);
    break;

  case kVpiPlusOp:
  case kVpiMinusOp:
    p = mAlloc.get_memory(sizeof(EiUnaryArithOp));
    expr = new (p) EiUnaryArithOp(pt_expr, opr1);
    break;

  case kVpiUnaryAndOp:
  case kVpiUnaryNandOp:
  case kVpiUnaryOrOp:
  case kVpiUnaryNorOp:
  case kVpiUnaryXorOp:
  case kVpiUnaryXNorOp:
    p = mAlloc.get_memory(sizeof(EiReductionOp));
    expr = new (p) EiReductionOp(pt_expr, opr1);
    break;

  case kVpiNotOp:
    p = mAlloc.get_memory(sizeof(EiNotOp));
    expr = new (p) EiNotOp(pt_expr, opr1);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiUnaryOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiUnaryOp::EiUnaryOp(const PtExpr* pt_expr,
		     ElbExpr* opr1) :
  EiOperation(pt_expr),
  mOpr1(opr1)
{
}

// @brief デストラクタ
EiUnaryOp::~EiUnaryOp()
{
}

// @brief 定数の時 true を返す．
// @note オペランドが定数ならこの式も定数となる．
bool
EiUnaryOp::is_const() const
{
  return mOpr1->is_const();
}

// @brief オペランド数を返す．
ymuint
EiUnaryOp::operand_num() const
{
  return 1;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
ElbExpr*
EiUnaryOp::_operand(ymuint pos) const
{
  return mOpr1;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNotOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiNotOp::EiNotOp(const PtExpr* pt_expr,
		 ElbExpr* opr1) :
  EiUnaryOp(pt_expr, opr1)
{
  // オペランドのサイズは self determined
  opr1->set_selfsize();
}

// @brief デストラクタ
EiNotOp::~EiNotOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiNotOp::value_type() const
{
  // 結果は常に符号無し1ビット
  return pack(kVpiValueUS, 1);
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiNotOp::set_reqsize(tVpiValueType type)
{
  // この演算子は型が固定なので何もしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitNeg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiBitNegOp::EiBitNegOp(const PtExpr* pt_expr,
		       ElbExpr* opr1) :
  EiUnaryOp(pt_expr, opr1)
{
  // オペランドの型とサイズをそのまま使う．
  mType = opr1->value_type();

  assert_cond(mType != kVpiValueReal, __FILE__, __LINE__);
}

// @brief デストラクタ
EiBitNegOp::~EiBitNegOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiBitNegOp::value_type() const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiBitNegOp::set_reqsize(tVpiValueType type)
{
  mType = update_size(mType, type);
  operand1()->set_reqsize(mType);
}


//////////////////////////////////////////////////////////////////////
// クラス EiReductionOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiReductionOp::EiReductionOp(const PtExpr* pt_expr,
			     ElbExpr* opr1) :
  EiUnaryOp(pt_expr, opr1)
{
  tVpiValueType type = opr1->value_type();
  assert_cond(type != kVpiValueReal, __FILE__, __LINE__);

  // オペランドのサイズは self determined
  opr1->set_selfsize();
}

// @brief デストラクタ
EiReductionOp::~EiReductionOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiReductionOp::value_type() const
{
  // 結果は常に符号無し1ビット
  return pack(kVpiValueUS, 1);
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiReductionOp::set_reqsize(tVpiValueType type)
{
  // この演算子は型が固定なので何もしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiUnaryArithOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiUnaryArithOp::EiUnaryArithOp(const PtExpr* pt_expr,
			       ElbExpr* opr1) :
  EiUnaryOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiUnaryArithOp::~EiUnaryArithOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiUnaryArithOp::value_type() const
{
  // オペランドの型とサイズをそのまま使う．
  tVpiValueType type = operand1()->value_type();
  return type;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiUnaryArithOp::set_reqsize(tVpiValueType type)
{
  operand1()->set_reqsize(type);
}


//////////////////////////////////////////////////////////////////////
// クラス EiEventEdgeOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiEventEdgeOp::EiEventEdgeOp(const PtExpr* pt_expr,
			     ElbExpr* opr1) :
  EiUnaryOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiEventEdgeOp::~EiEventEdgeOp()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiEventEdgeOp::value_type() const
{
  return kVpiValueNone;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiEventEdgeOp::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
