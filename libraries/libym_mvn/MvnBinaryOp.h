#ifndef LIBYM_MVN_MVNBINARYOP_H
#define LIBYM_MVN_MVNBINARYOP_H

/// @file libym_mvn/MvnBinaryOp.h
/// @brief MvnBinaryOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvnNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnBinaryOp MvnBinaryOp.h "MvnBinaryOp.h"
/// @brief 1入力1出力ノードの基底クラス
//////////////////////////////////////////////////////////////////////
class MvnBinaryOp :
  public MvnNode
{
protected:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width1 入力1のビット幅
  /// @param[in] bit_width2 入力2のビット幅
  /// @param[in] bit_width3 出力のビット幅
  MvnBinaryOp(MvnModule* module,
	      ymuint bit_width1,
	      ymuint bit_width2,
	      ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnBinaryOp();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNBINARYOP_H
