#ifndef EUFEQ_H
#define EUFEQ_H

/// @file EufEq.h
/// @brief EufEq のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufBin.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufEq EufEq.h "EufEq.h"
/// @brief euqality ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class EufEq :
  public EufBin
{
  friend class EufMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] left, right 左辺と右辺の式
  EufEq(EufNode* left,
	EufNode* right);

  /// @brief デストラクタ
  virtual
  ~EufEq();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  tType
  type() const;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFEQ_H
