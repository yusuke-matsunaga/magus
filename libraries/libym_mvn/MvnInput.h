#ifndef LIBYM_MVN_MVNINPUT_H
#define LIBYM_MVN_MVNINPUT_H

/// @file libym_mvn/MvnInput.h
/// @brief MvnInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvnNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnInput MvnInput.h "MvnInput.h"
/// @brief 入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnInput :
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
  MvnInput(MvnModule* module,
	   ymuint bit_width);

  /// @brief デストラクタ
  ~MvnInput();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNINPUT_H
