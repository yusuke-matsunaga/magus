#ifndef DOPTVLIST_H
#define DOPTVLIST_H

/// @file DopTvList.h
/// @brief DopTvList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DopTvList DopTvList.h "DopTvList.h"
/// @brief DetectOp の基本的な動作を行なうクラス
//////////////////////////////////////////////////////////////////////
class DopTvList :
  public DetectOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] tvlist テストベクタのリスト
  DopTvList(vector<TestVector*>& tvlist);

  /// @brief デストラクタ
  virtual
  ~DopTvList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった時の処理
  /// @param[in] f 故障
  /// @param[in] tv テストパタン
  virtual
  void
  operator()(TpgFault* f,
	     TestVector* tv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テストベクタのリスト
  vector<TestVector*>& mTvList;

};

END_NAMESPACE_YM_SATPG

#endif // DOPTVLIST_H
