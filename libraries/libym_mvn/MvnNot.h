#ifndef LIBYM_MVN_MVNNOT_H
#define LIBYM_MVN_MVNNOT_H

/// @file libym_mvn/MvnNot.h
/// @brief MvnNot のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnNot MvnNot.h "MvnNot.h"
/// @brief through (buffer) ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnNot :
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
  MvnNot(MvnModule* module,
	 ymuint bit_width);

  /// @brief デストラクタ
  ~MvnNot();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNNOT_H
