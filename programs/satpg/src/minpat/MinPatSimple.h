﻿#ifndef MINPATSIMPLE_H
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
  /// @param[in] fsim2 2値の故障シミュレータ(検証用)
  /// @param[out] fault_list 検出された故障のリスト
  virtual
  void
  init(TpgNetwork& network,
       TvMgr& tvmgr,
       Fsim& fsim2,
       vector<TpgFault*>& fault_list);

  /// @brief 最初の故障を選ぶ．
  virtual
  TpgFault*
  get_first_fault();

  /// @brief 次に処理すべき故障を選ぶ．
  /// @param[in] fgmgr 故障グループを管理するオブジェクト
  /// @param[in] group_list 現在のグループリスト
  ///
  /// 故障が残っていなければ NULL を返す．
  virtual
  TpgFault*
  get_next_fault(FgMgr& fgmgr,
		 const vector<ymuint>& group_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障リスト
  vector<TpgFault*> mFaultList;

  // 次の故障の位置
  ymuint mNextPos;

};

END_NAMESPACE_YM_SATPG

#endif // MINPATSIMPLE_H
