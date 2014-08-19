#ifndef EILHS_H
#define EILHS_H

/// @file EiLhs.h
/// @brief EiLhs のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiConcatOp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiLhs EiLhs.h "EiLhs.h"
/// @brief 連結演算子の左辺式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiLhs :
  public EiConcatOp
{
  friend class EiFactory;
private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr_size オペランド数
  /// @param[in] opr_array オペランドを格納する配列
  /// @param[in] lhs_elem_num 左辺の要素数
  /// @param[in] lhs_elem_array 左辺の要素の配列
  /// @note opr_array と lhs_elem_array は別物
  EiLhs(const PtExpr* pt_expr,
	ymuint opr_size,
	ElbExpr** opr_array,
	ymuint lhs_elem_num,
	ElbExpr** lhs_elem_array);

  /// @brief デストラクタ
  virtual
  ~EiLhs();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  virtual
  ymuint
  lhs_elem_num() const;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  virtual
  const VlExpr*
  lhs_elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint mNum;

  // 要素の配列
  ElbExpr** mArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EILHS_H
