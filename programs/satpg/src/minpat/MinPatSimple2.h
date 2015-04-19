#ifndef MINPATSIMPLE2_H
#define MINPATSIMPLE2_H

/// @file MinPatSimple2.h
/// @brief MinPatSimple2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatNaive.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPatSimple2 MinPatSimple2.h "MinPatSimple2.h"
/// @brief MinPatNaive の故障の順番を改良したクラス
//////////////////////////////////////////////////////////////////////
class MinPatSimple2 :
  public MinPatNaive
{
public:

  /// @brief コンストラクタ
  /// @param[in] group_dominance グループ支配を計算する．
  MinPatSimple2(bool group_dominance);

  /// @brief デストラクタ
  virtual
  ~MinPatSimple2();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_SATPG

#endif // MINPATSIMPLE2_H
