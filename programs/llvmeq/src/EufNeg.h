#ifndef EUFNEG_H
#define EUFNEG_H

/// @file EufNeg.h
/// @brief EufNeg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufNode.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufNeg EufNeg.h "EufNeg.h"
/// @brief EUF 式の否定演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EufNeg :
  public EufNode
{
  friend class EufMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] operand オペランド
  EufNeg(EufNode* operand);

  /// @brief デストラクタ
  virtual
  ~EufNeg();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  tType
  type() const;

  /// @brief 左辺の式を得る．
  /// @note type() が kCon, kDis, kNeg, kEq の時のみ有効
  virtual
  EufNode*
  left() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  EufNode* mLeft;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFNEG_H
