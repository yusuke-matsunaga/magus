#ifndef LIBYM_MVN_MVNOUTPUT_H
#define LIBYM_MVN_MVNOUTPUT_H

/// @file libym_mvn/MvnOutput.h
/// @brief MvnOutput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvnNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnOutput MvnOutput.h "MvnOutput.h"
/// @brief 出力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnOutput :
  public MvnNode
{
  friend class MvnMgr;

public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width ビット幅
  MvnOutput(MvnModule* module,
	    ymuint bit_width);

  /// @brief デストラクタ
  ~MvnOutput();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNOUTPUT_H
