#ifndef LIBYM_NETWORKS_MVNEQX_H
#define LIBYM_NETWORKS_MVNEQX_H

/// @file libym_networks/MvnEqX.h
/// @brief MvnConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class MvnEqX MvnEqX.h "MvnEqX.h"
/// @brief Xマスク付きの等価比較ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnEqX :
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
  MvnEqX(MvnModule* module,
	 const vector<ymuint32>& val);

  /// @brief デストラクタ
  ~MvnEqX();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Xマスクを得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kEqX の時のみ意味を持つ．
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  xmask(vector<ymuint32>& val) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  vector<ymuint32> mVal;

};

END_NAMESPACE_YM_NETWORKS

#endif // LIBYM_NETWORKS_MVNEQX_H
