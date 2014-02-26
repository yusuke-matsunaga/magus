#ifndef MINPAT_H
#define MINPAT_H

/// @file MinPat.h
/// @brief MinPat のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPat MinPat.h "MinPat.h"
/// @brief テストベクタの最小化を行うクラス
//////////////////////////////////////////////////////////////////////
class MinPat
{
public:

  /// @brief デストラクタ
  virtual
  ~MinPat() {}


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストベクタの最小化を行なう．
  /// @param[in] tv_list テストベクタのリスト
  /// @param[in] stats 実行結果の情報を格納する変数
  virtual
  void
  run(vector<TestVector*>& tv_list,
      MinPatStats& stats) = 0;

};


/// @brief インスタンスを生成する関数
extern
MinPat*
new_MinPat(AtpgMgr& mgr);

END_NAMESPACE_YM_SATPG

#endif // MINPAT_H
