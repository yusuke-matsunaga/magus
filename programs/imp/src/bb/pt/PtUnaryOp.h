#ifndef SRC_PT_PTUNARYOP_H
#define SRC_PT_PTUNARYOP_H

/// @file src/pt/PtUnaryOp.h
/// @brief PtUnaryOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class PtUnaryOp PtUnaryOp.h "PtUnaryOp.h"
/// @brief 単項演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class PtUnaryOp :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  /// @param[in] opr1 オペランド
  PtUnaryOp(const FileRegion& file_region,
	    PtNode* opr1);

  /// @brief デストラクタ
  virtual
  ~PtUnaryOp();


public:
  //////////////////////////////////////////////////////////////////////
  // PtNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オペランド数を返す．
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
  virtual
  PtNode*
  operand(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  PtNode* mOpr1;

};

END_NAMESPACE_YM_BB
#endif // SRC_PT_PTUNARYOP_H
