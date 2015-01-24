
/// @file YmslOpExpr.cc
/// @brief YmslOpExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslOpExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslOpExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
// @param[in] operand_list オペランドのリスト
YmslOpExpr::YmslOpExpr(ExprType type,
		       const vector<YmslExpr*>& operand_list) :
  mType(type)
{
  mOperandNum = operand_list.size();
  if ( mOperandNum > 0 ) {
    mOperandList = new YmslExpr*[mOperandNum];
    for (ymuint i = 0; i < mOperandNum; ++ i) {
      mOperandList[i] = operand_list[i];
    }
  }
  else {
    mOperandList = NULL;
  }
}

// @brief コンストラクタ
// @param[in] type 型
// @param[in] opr1 オペランド
//
// 単項演算用
YmslOpExpr::YmslOpExpr(ExprType type,
		       YmslExpr* opr1) :
  mType(type)
{
  mOperandNum = 1;
  mOperandList = new YmslExpr*[mOperandNum];
  mOperandList[0] = opr1;
}

// @brief コンストラクタ
// @param[in] type 型
// @param[in] opr1, opr2 オペランド
//
// 二項演算用
YmslOpExpr::YmslOpExpr(ExprType type,
		       YmslExpr* opr1,
		       YmslExpr* opr2) :
  mType(type)
{
  mOperandNum = 2;
  mOperandList = new YmslExpr*[mOperandNum];
  mOperandList[0] = opr1;
  mOperandList[1] = opr2;
}

// @brief コンストラクタ
// @param[in] type 型
// @param[in] opr1, opr2, opr3 オペランド
//
// 三項演算用
YmslOpExpr::YmslOpExpr(ExprType type,
		       YmslExpr* opr1,
		       YmslExpr* opr2,
		       YmslExpr* opr3) :
  mType(type)
{
  mOperandNum = 3;
  mOperandList = new YmslExpr*[mOperandNum];
  mOperandList[0] = opr1;
  mOperandList[1] = opr2;
  mOperandList[2] = opr3;
}

// @brief デストラクタ
YmslOpExpr::~YmslOpExpr()
{
  delete [] mOperandList;
}

// @brief 種類を返す．
ExprType
YmslOpExpr::expr_type() const
{
  return mType;
}

// @brief オペランドの数を返す．
//
// 演算子の時のみ有効
ymuint
YmslOpExpr::operand_num() const
{
  return mOperandNum;
}

// @brief オペランドを返す．
// @param[in] pos 位置 ( 0 <= pos < operand_num() )
//
// 演算子の時のみ有効
YmslExpr*
YmslOpExpr::operand(ymuint pos) const
{
  ASSERT_COND( pos < operand_num() );
  return mOperandList[pos];
}

END_NAMESPACE_YM_YMSL
