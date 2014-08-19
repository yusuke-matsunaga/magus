
/// @file EiLhs.cc
/// @brief EiLhs の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiLhs.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 連結演算子の左辺式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] opr_size オペランド数
// @param[in] opr_array オペランドを格納する配列
// @param[in] lhs_elem_num 左辺の要素数
// @param[in] lhs_elem_array 左辺の要素の配列
ElbExpr*
EiFactory::new_Lhs(const PtExpr* pt_expr,
		   ymuint opr_size,
		   ElbExpr** opr_array,
		   ymuint lhs_elem_num,
		   ElbExpr** lhs_elem_array)
{
  void* p = mAlloc.get_memory(sizeof(EiLhs));
  return new (p) EiLhs(pt_expr, opr_size, opr_array,
		       lhs_elem_num, lhs_elem_array);
}


//////////////////////////////////////////////////////////////////////
// クラス EiLhs
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] opr_size オペランド数
// @param[in] opr_array オペランドを格納する配列
// @param[in] lhs_elem_num 左辺の要素数
// @param[in] lhs_elem_array 左辺の要素の配列
// @note opr_array と lhs_elem_array は別物
EiLhs::EiLhs(const PtExpr* pt_expr,
	     ymuint opr_size,
	     ElbExpr** opr_array,
	     ymuint lhs_elem_num,
	     ElbExpr** lhs_elem_array) :
  EiConcatOp(pt_expr, opr_size, opr_array),
  mNum(lhs_elem_num),
  mArray(lhs_elem_array)
{
}

// @brief デストラクタ
EiLhs::~EiLhs()
{
  // mArray は別のオブジェクトが管理している．
}

// @brief 左辺式の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
ymuint
EiLhs::lhs_elem_num() const
{
  return mNum;
}

// @brief 左辺式の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
const VlExpr*
EiLhs::lhs_elem(ymuint pos) const
{
  return mArray[pos];
}

END_NAMESPACE_YM_VERILOG
