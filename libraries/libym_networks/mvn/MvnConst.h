#ifndef LIBYM_NETWORKS_MVNCONST_H
#define LIBYM_NETWORKS_MVNCONST_H

/// @file libym_networks/MvnConst.h
/// @brief MvnConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class MvnConst MvnConst.h "MvnConst.h"
/// @brief 入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnConst :
  public MvnNodeBase
{
  friend class MvnMgr;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] val 値
  MvnConst(MvnModule* module,
	   const vector<ymuint32>& val);

  /// @brief デストラクタ
  ~MvnConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数値を得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kConst の時のみ意味を持つ．
  virtual
  void
  const_value(vector<ymuint32>& val) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  vector<ymuint32> mVal;

};

END_NAMESPACE_YM_NETWORKS

#endif // LIBYM_NETWORKS_MVNCONST_H
