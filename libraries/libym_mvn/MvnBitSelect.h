#ifndef LIBYM_MVN_MVNBITSELECT_H
#define LIBYM_MVN_MVNBITSELECT_H

/// @file libym_mvn/MvnBitSelect.h
/// @brief MvnBitSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnBitSelect MvnBitSelect.h "MvnBitSelect.h"
/// @brief 可変 bit-select 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnBitSelect :
  public MvnBinaryOp
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
  /// @param[in] bit_width1 入力のビット幅
  /// @param[in] bit_width2 ビット指定入力のビット幅
  MvnBitSelect(MvnModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2);

  /// @brief デストラクタ
  ~MvnBitSelect();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNBITSELECT_H
