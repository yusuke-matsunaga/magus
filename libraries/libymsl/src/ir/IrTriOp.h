#ifndef IRTRIOP_H
#define IRTRIOP_H

/// @file IrTriOp.h
/// @brief IrTriOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrTriOp IrTriOp.h "IrTriOp.h"
/// @brief 三項演算を表すノード
//////////////////////////////////////////////////////////////////////
class IrTriOp :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] src1, src2, src3 オペランド
  IrTriOp(OpCode opcode,
	  IrNode* src1,
	  IrNode* src2,
	  IrNode* src3);

  /// @brief デストラクタ
  virtual
  ~IrTriOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief opcode を返す．
  virtual
  OpCode
  opcode() const;

  /// @brief 第1ソースを返す．
  virtual
  IrNode*
  src1() const;

  /// @brief 第2ソースを返す．
  virtual
  IrNode*
  src2() const;

  /// @brief 第3ソースを返す．
  virtual
  IrNode*
  src3() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // opcode
  OpCode mOpCode;

  // 第1ソース
  IrNode* mSrc1;

  // 第2ソース
  IrNode* mSrc2;

  // 第3ソース
  IrNode* mSrc3;

};

END_NAMESPACE_YM_YMSL

#endif // IRTRIOP_H
