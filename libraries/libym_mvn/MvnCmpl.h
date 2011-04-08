#ifndef LIBYM_MVN_MVNCMPL_H
#define LIBYM_MVN_MVNCMPL_H

/// @file libym_mvn/MvnCmpl.h
/// @brief MvnCmpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnCmpl MvnCmpl.h "MvnCmpl.h"
/// @brief complement(2の補数) ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnCmpl :
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
  MvnCmpl(MvnModule* module,
	  ymuint bit_width);

  /// @brief デストラクタ
  ~MvnCmpl();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVCMPL_H
