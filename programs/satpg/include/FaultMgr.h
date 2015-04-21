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
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FaultMgr FaultMgr.h "FaultMgr.h"
/// @brief 故障管理するすクラス
///
/// 故障には以下の状態がある．
/// - 未検出
/// - 検出済み
/// - 検出不能
/// FaultMgr はそれぞれの状態ごとの故障リストを持つ．
/// 故障の状態変化が FaultMgr::set_status() によって
/// 通知されるとその内容にしたがって故障リストを変更する．
///
/// ただし故障リストの更新は実際に読み出されるまで遅延される．
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

  /// @brief 故障の状態を得る．
  FaultStatus
  status(const TpgFault* fault) const;

  /// @brief 検出済みの代表故障のリストを得る．
  const vector<const TpgFault*>&
  det_list() const;

  /// @brief 検出済みの代表故障数を得る．
  ymuint
  det_num() const;

  /// @brief 未検出の代表故障のリストを得る．
  const vector<const TpgFault*>&
  remain_list() const;

  /// @brief 未検出の代表故障数を得る．
  ymuint
  remain_num() const;

  /// @brief 検出不能故障のリストを得る．
  const vector<const TpgFault*>&
  untest_list() const;

  /// @brief 検出不能故障数を得る．
  ymuint
  untest_num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を書き換えるメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear();

  /// @brief network の故障を設定する．
  /// @param[in] network 対象のネットワーク
  void
  set_faults(const TpgNetwork& network);

  /// @brief fault の状態を変更する．
  /// @param[in] fault 対象の故障
  /// @param[in] stat 故障の状態
  void
  set_status(const TpgFault* fault,
	     FaultStatus stat);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障リストをスキャンして未検出リストを更新する．
  void
  update() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障の状態を保持する配列
  vector<FaultStatus> mStatusArray;

  // 検出済みの故障を保持しておくリスト
  mutable
  vector<const TpgFault*> mDetList;

  // 未検出の故障を保持しておくリスト
  mutable
  vector<const TpgFault*> mRemainList;

  // 検出不能故障を保持しておくリスト
  mutable
  vector<const TpgFault*> mUntestList;

  // 故障リストに変化があったことを記録するフラグ
  mutable
  bool mChanged;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 故障の状態を得る．
inline
FaultStatus
FaultMgr::status(const TpgFault* fault) const
{
  ASSERT_COND( fault->id() < mStatusArray.size() );
  return mStatusArray[fault->id()];
}

// @brief 検出済みの代表故障のリストを得る．
inline
const vector<const TpgFault*>&
FaultMgr::det_list() const
{
  update();
  return mDetList;
}

// @brief 検出済みの代表故障数を得る．
inline
ymuint
FaultMgr::det_num() const
{
  update();
  return mDetList.size();
}

// @brief 未検出の故障のリストを得る．
inline
const vector<const TpgFault*>&
FaultMgr::remain_list() const
{
  update();
  return mRemainList;
}

// @brief 未検出の代表故障数を得る．
inline
ymuint
FaultMgr::remain_num() const
{
  update();
  return mRemainList.size();
}

// @brief 検出不能故障のリストを得る．
inline
const vector<const TpgFault*>&
FaultMgr::untest_list() const
{
  update();
  return mUntestList;
}

// @brief 検出不能故障数を得る．
inline
ymuint
FaultMgr::untest_num() const
{
  update();
  return mUntestList.size();
}

END_NAMESPACE_YM_SATPG

#endif // FAULTMGR_H
