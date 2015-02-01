#ifndef IRBINOP_H
#define IRBINOP_H

/// @file IrBinOp.h
/// @brief IrBinOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrBinOp IrBinOp.h "IrBinOp.h"
/// @brief 二項演算を表すノード
//////////////////////////////////////////////////////////////////////
class IrBinOp :
  public IrNode
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

  /// @brief 第1ソースを返す．
  virtual
  IrNode*
  src1() const;

  /// @brief 第2ソースを返す．
  virtual
  IrNode*
  src2() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 第1ソース
  IrNode* mSrc1;

  // 第2ソース
  IrNode* mSrc2;

};

END_NAMESPACE_YM_YMSL

#endif // IRBINOP_H
