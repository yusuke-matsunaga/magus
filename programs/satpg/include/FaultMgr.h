#ifndef FAULTMGR_H
#define FAULTMGR_H

/// @file FaultMgr.h
/// @brief FaultMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "satpg_nsdef.h"
#include "TpgFault.h"
#include "utils/SimpleAlloc.h"
#include "utils/UnitAlloc.h"


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

  /// @brief すべての故障数を得る．
  ymuint
  all_num() const;

  /// @brief 故障を取り出す．
  /// @param[in] id 故障番号 ( 0 <= id < all_num() )
  TpgFault*
  fault(ymuint id);

  /// @brief すべての代表故障数を得る．
  ymuint
  rep_num() const;

  /// @brief 代表故障を取り出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < rep_num() )
  TpgFault*
  rep_fault(ymuint pos);

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
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力の故障を生成する．
  /// @param[in] node 対象のノード
  /// @param[in] val 縮退している値
  /// @param[in] rep 代表故障
  /// @param[inout] fid 故障のID番号
  /// @note fid はこの関数呼び出し後に1つインクリメントされる．
  TpgFault*
  new_ofault(TpgNode* node,
	     int val,
	     TpgFault* rep,
	     ymuint& fid);

  /// @brief 入力の故障を生成する．
  /// @param[in] node 対象のノード
  /// @param[in] pos 入力番号を表す
  /// @param[in] val 縮退している値
  /// @param[in] rep 代表故障
  /// @param[inout] fid 故障のID番号
  /// @note fid はこの関数呼び出し後に1つインクリメントされる．
  TpgFault*
  new_ifault(TpgNode* node,
	     ymuint pos,
	     int val,
	     TpgFault* rep,
	     ymuint& fid);

  /// @brief 故障を生成する．
  /// @param[in] node 対象のノード
  /// @param[in] is_output 出力の故障のときに true とするフラグ
  /// @param[in] pos 入力の故障の時に入力番号を表す
  /// @param[in] val 縮退している値
  /// @param[in] rep 代表故障
  /// @param[inout] fid 故障のID番号
  /// @note fid はこの関数呼び出し後に1つインクリメントされる．
  TpgFault*
  new_fault(TpgNode* node,
	    bool is_output,
	    ymuint pos,
	    int val,
	    TpgFault* rep,
	    ymuint& fid);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関連付けられているネットワーク
  TpgNetwork* mNetwork;

  // 全故障数
  ymuint32 mAllNum;

  // 全ての故障をまとめて確保したもの
  TpgFault* mFaultChunk;

  // 代表故障数
  ymuint32 mRepNum;

  // 代表故障のポインタの配列
  TpgFault** mRepArray;

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

// @brief すべての故障数を得る．
inline
ymuint
FaultMgr::all_num() const
{
  return mAllNum;
}

// @brief 故障を取り出す．
// @param[in] id 故障番号 ( 0 <= id < all_num() )
inline
TpgFault*
FaultMgr::fault(ymuint id)
{
  assert_cond( id < all_num(), __FILE__, __LINE__);
  return &mFaultChunk[id];
}

// @brief すべての代表故障数を得る．
inline
ymuint
FaultMgr::rep_num() const
{
  return mRepNum;
}

// @brief 代表故障を取り出す．
// @param[in] pos 位置番号 ( 0 <= pos < rep_num() )
inline
TpgFault*
FaultMgr::rep_fault(ymuint pos)
{
  assert_cond( pos < rep_num(), __FILE__, __LINE__);
  return mRepArray[pos];
}

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

// @brief 出力の故障を生成する．
// @param[in] node 対象のノード
// @param[in] val 縮退している値
// @param[in] rep 代表故障
// @param[inout] fid 故障のID番号
// @note fid はこの関数呼び出し後に1つインクリメントされる．
inline
TpgFault*
FaultMgr::new_ofault(TpgNode* node,
		     int val,
		     TpgFault* rep,
		     ymuint& fid)
{
  return new_fault(node, true, 0, val, rep, fid);
}

// @brief 入力の故障を生成する．
// @param[in] node 対象のノード
// @param[in] pos 入力番号を表す
// @param[in] val 縮退している値
// @param[in] rep 代表故障
// @param[inout] fid 故障のID番号
// @note fid はこの関数呼び出し後に1つインクリメントされる．
inline
TpgFault*
FaultMgr::new_ifault(TpgNode* node,
		     ymuint pos,
		     int val,
		     TpgFault* rep,
		     ymuint& fid)
{
  return new_fault(node, false, pos, val, rep, fid);
}

END_NAMESPACE_YM_SATPG

#endif // FAULTMGR_H
