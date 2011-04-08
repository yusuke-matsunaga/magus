#ifndef LIBYM_MVN_MVNITE_H
#define LIBYM_MVN_MVNITE_H

/// @file libym_mvn/MvnIte.h
/// @brief MvnIte のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnTernaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnIte MvnIte.h "MvnIte.h"
/// @brief ITE 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnIte :
  public MvnTernaryOp
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
  MvnIte(MvnModule* module,
	 ymuint bit_width);

  /// @brief デストラクタ
  ~MvnIte();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNITE_H
