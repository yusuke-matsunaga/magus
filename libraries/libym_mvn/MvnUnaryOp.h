#ifndef LIBYM_MVN_MVNUNARYOP_H
#define LIBYM_MVN_MVNUNARYOP_H

/// @file libym_mvn/MvnUnaryOp.h
/// @brief MvnUnaryOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvnNode.h"
#include "ym_mvn/MvnPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnUnaryOp MvnUnaryOp.h "MvnUnaryOp.h"
/// @brief 1入力1出力ノードの基底クラス
//////////////////////////////////////////////////////////////////////
class MvnUnaryOp :
  public MvnNode
{
protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力のビット幅
  /// @param[in] bit_width2 出力のビット幅
  MvnUnaryOp(MvnModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2);

  /// @brief デストラクタ
  ~MvnUnaryOp();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNUNARYOP_H
