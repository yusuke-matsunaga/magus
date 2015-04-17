#ifndef MINPATSIMPLE_H
#define MINPATSIMPLE_H

/// @file MinPatSimple.h
/// @brief MinPatSimple のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPatSimple MinPatSimple.h "MinPatSimple.h"
/// @brief MinPatNaive の故障の順番を改良したクラス
//////////////////////////////////////////////////////////////////////
class MinPatSimple :
  public MinPatBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] group_dominance グループ支配を計算する．
  MinPatSimple(bool group_dominance);

  /// @brief デストラクタ
  virtual
  ~MinPatSimple();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


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
       vector<TestVector*>& tv_list);

  /// @brief 最初の故障を選ぶ．
  virtual
  TpgFault*
  get_first_fault();

  /// @brief 次に処理すべき故障を選ぶ．
  /// @param[in] fgmgr 故障グループを管理するオブジェクト
  ///
  /// 故障が残っていなければ NULL を返す．
  virtual
  TpgFault*
  get_next_fault(FgMgr& fgmgr);

  /// @brief 故障を追加するグループを選ぶ．
  /// @param[in] fgmgr 故障グループを管理するオブジェクト
  /// @param[in] fault 故障
  ///
  /// グループが見つからなければ fgmgr.group_num() を返す．
  virtual
  ymuint
  find_group(FgMgr& fgmgr,
	     TpgFault* fault);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号の最大値
  ymuint mMaxNodeId;

  // 故障リスト
  vector<TpgFault*> mFaultList;

  // 次の故障の位置
  ymuint mNextPos;

};

END_NAMESPACE_YM_SATPG

#endif // MINPATSIMPLE_H
