#ifndef MINPATBASE_H
#define MINPATBASE_H

/// @file MinPatBase.h
/// @brief MinPatBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPat.h"


BEGIN_NAMESPACE_YM_SATPG

class FgMgr;

//////////////////////////////////////////////////////////////////////
/// @class MinPatBase MinPatBase.h "MinPatBase.h"
/// @brief MinPat の実装クラス
//////////////////////////////////////////////////////////////////////
class MinPatBase :
  public MinPat
{
public:

  /// @brief コンストラクタ
  /// @param[in] group_dominance グループ支配を計算する．
  /// @param[in] fault_dominace 故障支配を計算する．
  MinPatBase(bool group_dominance = false,
	     bool fault_dominance = false);

  /// @brief デストラクタ
  virtual
  ~MinPatBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストベクタの最小化を行なう．
  /// @param[in] network 対象のネットワーク
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim2 2値の故障シミュレータ(検証用)
  /// @param[out] tv_list テストベクタのリスト
  /// @param[out] stats 実行結果の情報を格納する変数
  virtual
  void
  run(TpgNetwork& network,
      TvMgr& tvmgr,
      FaultMgr& fmgr,
      Fsim& fsim2,
      vector<TestVector*>& tv_list,
      MinPatStats& stats);

  /// @brief verbose フラグをセットする．
  virtual
  void
  set_verbose(int verbose);

  /// @brief verbose フラグを得る．
  int
  verbose() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim2 2値の故障シミュレータ(検証用)
  /// @param[in] tv_list テストベクタのリスト
  /// @return 支配故障数を返す．
  virtual
  ymuint
  init(TpgNetwork& network,
       TvMgr& tvmgr,
       FaultMgr& fmgr,
       Fsim& fsim2,
       vector<TestVector*>& tv_list) = 0;

  /// @brief 最初の故障を選ぶ．
  virtual
  TpgFault*
  get_first_fault() = 0;

  /// @brief 次に処理すべき故障を選ぶ．
  /// @param[in] fgmgr 故障グループを管理するオブジェクト
  ///
  /// 故障が残っていなければ NULL を返す．
  virtual
  TpgFault*
  get_next_fault(FgMgr& fgmgr) = 0;

  /// @brief 故障を追加するグループを選ぶ．
  /// @param[in] fgmgr 故障グループを管理するオブジェクト
  /// @param[in] fault 故障
  ///
  /// グループが見つからなければ fgmgr.group_num() を返す．
  virtual
  ymuint
  find_group(FgMgr& fgmgr,
	     TpgFault* fault) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // verbose フラグ
  int mVerbose;

  // グループ支配を計算するとき true にするフラグ
  bool mGroupDominance;

  // 故障支配を計算する時 true にするフラグ
  bool mFaultDominance;

};

END_NAMESPACE_YM_SATPG

#endif // MINPATBASE_H
