#ifndef LIBYM_MVN_MVNPARTSELECT_H
#define LIBYM_MVN_MVNPARTSELECT_H

/// @file libym_mvn/MvnPartSelect.h
/// @brief MvnPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnBinaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnPartSelect MvnPartSelect.h "MvnPartSelect.h"
/// @brief 可変 part-select 演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnPartSelect :
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
  /// @param[in] bit_width2 範囲指定入力(base位置)のビット幅
  /// @param[in] bit_width3 範囲のビット幅(出力のビット幅)
  MvnPartSelect(MvnModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3);

  /// @brief デストラクタ
  ~MvnPartSelect();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNPARTSELECT_H
