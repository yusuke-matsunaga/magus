#ifndef LIBYM_MVN_MVNTHROUGH_H
#define LIBYM_MVN_MVNTHROUGH_H

/// @file libym_mvn/MvnThrough.h
/// @brief MvnThrough のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnThrough MvnThrough.h "MvnThrough.h"
/// @brief through (buffer) ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnThrough :
  public MvnUnaryOp
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
  MvnThrough(MvnModule* module,
	     ymuint bit_width);

  /// @brief デストラクタ
  ~MvnThrough();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNTHROUGH_H
