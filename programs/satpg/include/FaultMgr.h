#ifndef FAULTMGR_H
#define FAULTMGR_H

/// @file FaultMgr.h
/// @brief FaultMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "satpg_nsdef.h"
#include "FaultStatus.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FaultMgr FaultMgr.h "FaultMgr.h"
/// @brief 故障リストを表すクラス
//////////////////////////////////////////////////////////////////////
class FaultMgr
{
public:

  /// @brief コンストラクタ
  FaultMgr();

  /// @brief デストラクタ
  ~FaultMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // read-only のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 検出済みの代表故障のリストを得る．
  const vector<TpgFault*>&
  det_list();

  /// @brief 未検出の代表故障のリストを得る．
  const vector<TpgFault*>&
  remain_list();

  /// @brief 未検出の代表故障数を得る．
  ymuint
  remain_num();

  /// @brief 検出不能故障のリストを得る．
  const vector<TpgFault*>&
  untest_list();

  /// @brief 検出不能故障数を得る．
  ymuint
  untest_num();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を書き換えるメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear();

  /// @brief network の全ての単一縮退故障を設定する．
  void
  set_ssa_fault(TpgNetwork& network);

  /// @brief fault の状態を変更する．
  void
  set_status(TpgFault* fault,
	     FaultStatus stat);

  /// @brief 故障リストをスキャンして未検出リストを更新する．
  void
  update();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 検出済みの故障を保持しておくリスト
  vector<TpgFault*> mDetList;

  // 未検出の故障を保持しておくリスト
  vector<TpgFault*> mRemainList;

  // 検出不能故障を保持しておくリスト
  vector<TpgFault*> mUntestList;

  // 故障リストに変化があったことを記録するフラグ
  bool mChanged;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 検出済みの代表故障のリストを得る．
inline
const vector<TpgFault*>&
FaultMgr::det_list()
{
  update();
  return mDetList;
}

// @brief 未検出の故障のリストを得る．
inline
const vector<TpgFault*>&
FaultMgr::remain_list()
{
  update();
  return mRemainList;
}

// @brief 未検出の代表故障数を得る．
inline
ymuint
FaultMgr::remain_num()
{
  update();
  return mRemainList.size();
}

// @brief 検出不能故障のリストを得る．
inline
const vector<TpgFault*>&
FaultMgr::untest_list()
{
  update();
  return mUntestList;
}

// @brief 検出不能故障数を得る．
inline
ymuint
FaultMgr::untest_num()
{
  update();
  return mUntestList.size();
}

END_NAMESPACE_YM_SATPG

#endif // FAULTMGR_H
