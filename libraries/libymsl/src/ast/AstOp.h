#ifndef ASTOP_H
#define ASTOP_H

/// @file AstOp.h
/// @brief AstOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @ckass AstOp AstOp.h "AstOp.h"
/// @brief 演算子を表す AstExpr
//////////////////////////////////////////////////////////////////////
class AstOp :
  public AstExpr
{
public:

  /// @breif コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] loc ファイル位置
  AstOp(OpCode opcode,
	const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オペコードを返す．
  ///
  /// 演算子のみ有効
  virtual
  OpCode
  opcode() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペコード
  OpCode mOpCode;

};

END_NAMESPACE_YM_YMSL

#endif // ASTOP_H
