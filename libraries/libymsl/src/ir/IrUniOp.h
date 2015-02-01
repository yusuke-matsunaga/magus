#ifndef IRUNIOP_H
#define IRUNIOP_H

/// @file IrUniOp.h
/// @brief IrUniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrUniOp IrUniOp.h "IrUniOp.h"
/// @brief 単項演算を表すノード
//////////////////////////////////////////////////////////////////////
class IrUniOp :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] type 型
  /// @param[in] src1 オペランド
  IrUniOp(OpCode opcode,
	  const Type* type,
	  IrNode* src1);

  /// @brief デストラクタ
  virtual
  ~IrUniOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 第1ソースを返す．
  virtual
  IrNode*
  src1() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 第1ソース
  IrNode* mSrc1;

};

END_NAMESPACE_YM_YMSL

#endif // IRUNIOP_H
