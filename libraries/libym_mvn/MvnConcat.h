#ifndef LIBYM_MVN_MVNCONCAT_H
#define LIBYM_MVN_MVNCONCAT_H

/// @file libym_mvn/MvnConcat.h
/// @brief MvnConcat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvnNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnConcat MvnConcat.h "MvnConcat.h"
/// @brief 連結演算を表すノード
//////////////////////////////////////////////////////////////////////
class MvnConcat :
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
  /// @param[in] ibitwidth_array 入力のビット幅の配列
  MvnConcat(MvnModule* module,
	   const vector<ymuint>& ibitwidth_array);

  /// @brief デストラクタ
  ~MvnConcat();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNCONCAT_H
