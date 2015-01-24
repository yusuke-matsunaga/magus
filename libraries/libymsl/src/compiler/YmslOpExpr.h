#ifndef YMSLOPEXPR_H
#define YMSLOPEXPR_H

/// @file YmslOpExpr.h
/// @brief YmslOpExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslOpExpr YmslOpExpr.h "YmslOpExpr.h"
/// @brief 汎用的な YmslExpr クラス
//////////////////////////////////////////////////////////////////////
class YmslOpExpr :
  public YmslExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] operand_list オペランドのリスト
  YmslOpExpr(ExprType type,
	     const vector<YmslExpr*>& operand_list);

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] opr1 オペランド
  ///
  /// 単項演算用
  YmslOpExpr(ExprType type,
	     YmslExpr* opr1);

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] opr1, opr2 オペランド
  ///
  /// 二項演算用
  YmslOpExpr(ExprType type,
	     YmslExpr* opr1,
	     YmslExpr* opr2);

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] opr1, opr2, opr3 オペランド
  ///
  /// 三項演算用
  YmslOpExpr(ExprType type,
	     YmslExpr* opr1,
	     YmslExpr* opr2,
	     YmslExpr* opr3);

  /// @brief デストラクタ
  virtual
  ~YmslOpExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief オペランドの数を返す．
  ///
  /// 演算子の時のみ有効
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置 ( 0 <= pos < operand_num() )
  ///
  /// 演算子の時のみ有効
  virtual
  YmslExpr*
  operand(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  ExprType mType;

  // オペランド数
  ymuint mOperandNum;

  // オペランドのリスト
  YmslExpr** mOperandList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLOPEXPR_H
