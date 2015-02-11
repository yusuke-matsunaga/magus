#ifndef IRBINOP_H
#define IRBINOP_H

/// @file IrBinOp.h
/// @brief IrBinOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrBinOp IrBinOp.h "IrBinOp.h"
/// @brief 二項演算を表すノード
//////////////////////////////////////////////////////////////////////
class IrBinOp :
  public IrOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] type 型
  /// @param[in] src1, src2 オペランド
  IrBinOp(OpCode opcode,
	  const Type* type,
	  IrNode* src1,
	  IrNode* src2);

  /// @brief デストラクタ
  virtual
  ~IrBinOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

  /// @brief オペランド数を返す．
  ///
  /// 演算子のみ有効
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置 ( 0 <= pos < operand_num() )
  ///
  /// 演算子のみ有効
  virtual
  IrNode*
  operand(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  IrNode* mOperand[2];

};

END_NAMESPACE_YM_YMSL

#endif // IRBINOP_H
