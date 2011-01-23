
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
EiFactory::new_UnaryOp(const PtBase* pt_expr,
		       tVpiOpType op_type,
		       ElbExpr* opr1)
{
  ElbExpr* expr = NULL;
  void* p;
  switch ( op_type ) {
  case kVpiPosedgeOp:
    p = mAlloc.get_memory(sizeof(EiPosedgeOp));
    expr = new (p) EiPosedgeOp(pt_expr, opr1);
    break;

  case kVpiNegedgeOp:
    p = mAlloc.get_memory(sizeof(EiNegedgeOp));
    expr = new (p) EiNegedgeOp(pt_expr, opr1);
    break;

  case kVpiBitNegOp:
    p = mAlloc.get_memory(sizeof(EiBitNegOp));
    expr = new (p) EiBitNegOp(pt_expr, opr1);
    break;

  case kVpiPlusOp:
    p = mAlloc.get_memory(sizeof(EiPlusOp));
    expr = new (p) EiPlusOp(pt_expr, opr1);
    break;

  case kVpiMinusOp:
    p = mAlloc.get_memory(sizeof(EiMinusOp));
    expr = new (p) EiMinusOp(pt_expr, opr1);
    break;

  case kVpiUnaryAndOp:
    p = mAlloc.get_memory(sizeof(EiUnaryAndOp));
    expr = new (p) EiUnaryAndOp(pt_expr, opr1);
    break;

  case kVpiUnaryNandOp:
    p = mAlloc.get_memory(sizeof(EiUnaryNandOp));
    expr = new (p) EiUnaryNandOp(pt_expr, opr1);
    break;

  case kVpiUnaryOrOp:
    p = mAlloc.get_memory(sizeof(EiUnaryOrOp));
    expr = new (p) EiUnaryOrOp(pt_expr, opr1);
    break;

  case kVpiUnaryNorOp:
    p = mAlloc.get_memory(sizeof(EiUnaryNorOp));
    expr = new (p) EiUnaryNorOp(pt_expr, opr1);
    break;

  case kVpiUnaryXorOp:
    p = mAlloc.get_memory(sizeof(EiUnaryXorOp));
    expr = new (p) EiUnaryXorOp(pt_expr, opr1);
    break;

  case kVpiUnaryXNorOp:
    p = mAlloc.get_memory(sizeof(EiUnaryXnorOp));
    expr = new (p) EiUnaryXnorOp(pt_expr, opr1);
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
EiUnaryOp::EiUnaryOp(const PtBase* pt_expr,
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
EiNotOp::EiNotOp(const PtBase* pt_expr,
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

// @brief 演算子のタイプを返す．
tVpiOpType
EiNotOp::op_type() const
{
  return kVpiNotOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitNeg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiBitNegOp::EiBitNegOp(const PtBase* pt_expr,
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

// @brief 演算子のタイプを返す．
tVpiOpType
EiBitNegOp::op_type() const
{
  return kVpiBitNegOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiReductionOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiReductionOp::EiReductionOp(const PtBase* pt_expr,
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
// クラス EiUnaryAndOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiUnaryAndOp::EiUnaryAndOp(const PtBase* pt_expr,
			   ElbExpr* opr1) :
  EiReductionOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiUnaryAndOp::~EiUnaryAndOp()
{
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiUnaryAndOp::op_type() const
{
  return kVpiUnaryAndOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiUnaryNandOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiUnaryNandOp::EiUnaryNandOp(const PtBase* pt_expr,
			     ElbExpr* opr1) :
  EiReductionOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiUnaryNandOp::~EiUnaryNandOp()
{
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiUnaryNandOp::op_type() const
{
  return kVpiUnaryNandOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiUnaryOrOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiUnaryOrOp::EiUnaryOrOp(const PtBase* pt_expr,
			 ElbExpr* opr1) :
  EiReductionOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiUnaryOrOp::~EiUnaryOrOp()
{
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiUnaryOrOp::op_type() const
{
  return kVpiUnaryOrOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiUnaryNorOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiUnaryNorOp::EiUnaryNorOp(const PtBase* pt_expr,
			   ElbExpr* opr1) :
  EiReductionOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiUnaryNorOp::~EiUnaryNorOp()
{
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiUnaryNorOp::op_type() const
{
  return kVpiUnaryNorOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiUnaryXorOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiUnaryXorOp::EiUnaryXorOp(const PtBase* pt_expr,
			   ElbExpr* opr1) :
  EiReductionOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiUnaryXorOp::~EiUnaryXorOp()
{
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiUnaryXorOp::op_type() const
{
  return kVpiUnaryXorOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiUnaryXnorOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiUnaryXnorOp::EiUnaryXnorOp(const PtBase* pt_expr,
			     ElbExpr* opr1) :
  EiReductionOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiUnaryXnorOp::~EiUnaryXnorOp()
{
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiUnaryXnorOp::op_type() const
{
  return kVpiUnaryXNorOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiUnaryArithOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiUnaryArithOp::EiUnaryArithOp(const PtBase* pt_expr,
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
// クラス EiPlusOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiPlusOp::EiPlusOp(const PtBase* pt_expr,
		   ElbExpr* opr1) :
  EiUnaryArithOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiPlusOp::~EiPlusOp()
{
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiPlusOp::op_type() const
{
  return kVpiPlusOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiMinusOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiMinusOp::EiMinusOp(const PtBase* pt_expr,
		     ElbExpr* opr1) :
  EiUnaryArithOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiMinusOp::~EiMinusOp()
{
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiMinusOp::op_type() const
{
  return kVpiMinusOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiEventEdgeOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiEventEdgeOp::EiEventEdgeOp(const PtBase* pt_expr,
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


//////////////////////////////////////////////////////////////////////
// クラス EiPosedgeOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiPosedgeOp::EiPosedgeOp(const PtBase* pt_expr,
			 ElbExpr* opr1) :
  EiEventEdgeOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiPosedgeOp::~EiPosedgeOp()
{
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiPosedgeOp::op_type() const
{
  return kVpiPosedgeOp;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNegedgeOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr1 オペランド
EiNegedgeOp::EiNegedgeOp(const PtBase* pt_expr,
			 ElbExpr* opr1) :
  EiEventEdgeOp(pt_expr, opr1)
{
}

// @brief デストラクタ
EiNegedgeOp::~EiNegedgeOp()
{
}

// @brief 演算子のタイプを返す．
tVpiOpType
EiNegedgeOp::op_type() const
{
  return kVpiNegedgeOp;
}

END_NAMESPACE_YM_VERILOG
