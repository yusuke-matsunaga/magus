#ifndef YMSLOPEXPR_H
#define YMSLOPEXPR_H

/// @file IrOpExpr.h
/// @brief IrOpExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrOpExpr IrOpExpr.h "IrOpExpr.h"
/// @brief 汎用的な IrExpr クラス
//////////////////////////////////////////////////////////////////////
class IrOpExpr :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] operand_list オペランドのリスト
  IrOpExpr(ExprType type,
	     const vector<IrExpr*>& operand_list);

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] opr1 オペランド
  ///
  /// 単項演算用
  IrOpExpr(ExprType type,
	     IrExpr* opr1);

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] opr1, opr2 オペランド
  ///
  /// 二項演算用
  IrOpExpr(ExprType type,
	     IrExpr* opr1,
	     IrExpr* opr2);

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] opr1, opr2, opr3 オペランド
  ///
  /// 三項演算用
  IrOpExpr(ExprType type,
	     IrExpr* opr1,
	     IrExpr* opr2,
	     IrExpr* opr3);

  /// @brief デストラクタ
  virtual
  ~IrOpExpr();


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
  IrExpr*
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
  IrExpr** mOperandList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLOPEXPR_H
