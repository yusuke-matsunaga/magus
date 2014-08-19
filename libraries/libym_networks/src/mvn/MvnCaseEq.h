#ifndef MVNCASEEQ_H
#define MVNCASEEQ_H

/// @file MvnCaseEq.h
/// @brief MvnCaseEq のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnCaseEq MvnCaseEq.h "MvnCaseEq.h"
/// @brief Xマスク付きの等価比較ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnCaseEq :
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
  MvnCaseEq(MvnModule* module,
	    const vector<ymuint32>& val);

  /// @brief デストラクタ
  ~MvnCaseEq();


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

END_NAMESPACE_YM_NETWORKS_MVN

#endif // MVNEQX_H
