
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

#include "ym_verilog/VlOpType.h"
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
		       const VlOpType& op_type,
		       ElbExpr* opr1)
{
  ElbExpr* expr = NULL;
  void* p;
  switch ( op_type.val() ) {
  case vpiPosedgeOp:
  case vpiNegedgeOp:
    p = mAlloc.get_memory(sizeof(EiEventEdgeOp));
    expr = new (p) EiEventEdgeOp(pt_expr, opr1);
    break;

  case vpiBitNegOp:
    p = mAlloc.get_memory(sizeof(EiBitNegOp));
    expr = new (p) EiBitNegOp(pt_expr, opr1);
    break;

  case vpiPlusOp:
  case vpiMinusOp:
    p = mAlloc.get_memory(sizeof(EiUnaryArithOp));
    expr = new (p) EiUnaryArithOp(pt_expr, opr1);
    break;

  case vpiUnaryAndOp:
  case vpiUnaryNandOp:
  case vpiUnaryOrOp:
  case vpiUnaryNorOp:
  case vpiUnaryXorOp:
  case vpiUnaryXNorOp:
    p = mAlloc.get_memory(sizeof(EiReductionOp));
    expr = new (p) EiReductionOp(pt_expr, opr1);
    break;

  case vpiNotOp:
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
VlValueType
EiNotOp::value_type() const
{
  // 結果は常に符号無し1ビット
  return VlValueType(false, true, 1);
}

// @brief オペランドに要求されるデータ型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
// @note kVpiOperation の時，意味を持つ．
// @note それ以外では kVpiValueNone を返す．
// 通常はオペランドの式の value_type() に一致するが，
// その式が self-typed の場合には異なることもある．
VlValueType
EiNotOp::operand_type(ymuint pos) const
{
  // 結果は常に符号なし1ビット
  return VlValueType(false, true, 1);
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiNotOp::set_reqsize(const VlValueType& type)
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

  assert_cond( !mType.is_real_type(), __FILE__, __LINE__);
}

// @brief デストラクタ
EiBitNegOp::~EiBitNegOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiBitNegOp::value_type() const
{
  return mType;
}

// @brief オペランドに要求されるデータ型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
// @note kVpiOperation の時，意味を持つ．
// @note それ以外では kVpiValueNone を返す．
// 通常はオペランドの式の value_type() に一致するが，
// その式が self-typed の場合には異なることもある．
VlValueType
EiBitNegOp::operand_type(ymuint pos) const
{
  return mType;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiBitNegOp::set_reqsize(const VlValueType& type)
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
  assert_cond( !opr1->value_type().is_real_type(), __FILE__, __LINE__);

  // オペランドのサイズは self determined
  opr1->set_selfsize();
}

// @brief デストラクタ
EiReductionOp::~EiReductionOp()
{
}

// @brief 式のタイプを返す．
VlValueType
EiReductionOp::value_type() const
{
  // 結果は常に符号無し1ビット
  return VlValueType(false, true, 1);
}

// @brief オペランドに要求されるデータ型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
// @note kVpiOperation の時，意味を持つ．
// @note それ以外では kVpiValueNone を返す．
// 通常はオペランドの式の value_type() に一致するが，
// その式が self-typed の場合には異なることもある．
VlValueType
EiReductionOp::operand_type(ymuint pos) const
{
  return operand1()->value_type();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiReductionOp::set_reqsize(const VlValueType& type)
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
VlValueType
EiUnaryArithOp::value_type() const
{
  // オペランドの型とサイズをそのまま使う．
  return operand1()->value_type();
}

// @brief オペランドに要求されるデータ型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
// @note kVpiOperation の時，意味を持つ．
// @note それ以外では kVpiValueNone を返す．
// 通常はオペランドの式の value_type() に一致するが，
// その式が self-typed の場合には異なることもある．
VlValueType
EiUnaryArithOp::operand_type(ymuint pos) const
{
  return operand1()->value_type();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiUnaryArithOp::set_reqsize(const VlValueType& type)
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
VlValueType
EiEventEdgeOp::value_type() const
{
  return VlValueType();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiEventEdgeOp::set_reqsize(const VlValueType& type)
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
