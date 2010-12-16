
/// @file libym_verilog/elb_impl/EiLhs.cc
/// @brief EiLhs の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiLhs.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiLhs.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

/// @brief 単純な左辺式を生成する．
/// @param[in] expr 対応する式
ElbLhs*
EiFactory::new_Lhs(ElbExpr* expr)
{
  void* q = mAlloc.get_memory(sizeof(EiLhsSimple));
  ElbLhs* lhs = new (q) EiLhsSimple(expr);

  return lhs;
}

// @brief 連結演算子の左辺式を生成する．
// @param[in] expr 対応する式
// @param[in] elem_array 要素のベクタ
ElbLhs*
EiFactory::new_Lhs(ElbExpr* expr,
		   const vector<ElbExpr*>& elem_array)
{
  ymuint n = elem_array.size();
  if ( n == 1 ) {
    assert_cond( expr == elem_array[0], __FILE__, __LINE__);
    return new_Lhs(expr);
  }
  void* p = mAlloc.get_memory(sizeof(ElbExpr*) * n);
  ElbExpr** array = new (p) ElbExpr*[n];
  for (ymuint i = 0; i < n; ++ i) {
    array[i] = elem_array[i];
  }
  void* q = mAlloc.get_memory(sizeof(EiLhsConcat));
  ElbLhs* lhs = new (q) EiLhsConcat(expr, n, array);

  return lhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiLhsSimple
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 対応する式
EiLhsSimple::EiLhsSimple(ElbExpr* expr) :
  ElbLhs(expr)
{
}

// @brief デストラクタ
EiLhsSimple::~EiLhsSimple()
{
}

// @brief 左辺式の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
ymuint
EiLhsSimple::elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
ElbExpr*
EiLhsSimple::elem(ymuint pos) const
{
  return _expr();
}


//////////////////////////////////////////////////////////////////////
// クラス EiLhsConcat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 対応する式
// @param[in] elem_num 要素数
// @param[in] elem_array 要素の配列
EiLhsConcat::EiLhsConcat(ElbExpr* expr,
			 ymuint elem_num,
			 ElbExpr** elem_array) :
  ElbLhs(expr),
  mNum(elem_num),
  mArray(elem_array)
{
}

// @brief デストラクタ
EiLhsConcat::~EiLhsConcat()
{
  // mArray は別のオブジェクトが管理している．
}

// @brief 左辺式の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
ymuint
EiLhsConcat::elem_num() const
{
  return mNum;
}

// @brief 左辺式の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
ElbExpr*
EiLhsConcat::elem(ymuint pos) const
{
  return mArray[pos];
}

END_NAMESPACE_YM_VERILOG
