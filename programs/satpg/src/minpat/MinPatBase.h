#ifndef MINPATBASE_H
#define MINPATBASE_H

/// @file MinPatBase.h
/// @brief MinPatBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPat.h"
#include "FaultAnalyzer.h"


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
  MinPatBase(bool group_dominance = false);

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
  /// @param[in] fsim2 2値の故障シミュレータ(検証用)
  /// @param[out] tv_list テストベクタのリスト
  /// @param[out] stats 実行結果の情報を格納する変数
  virtual
  void
  run(TpgNetwork& network,
      TvMgr& tvmgr,
      Fsim& fsim2,
      vector<TestVector*>& tv_list,
      USTime& time);

  /// @brief verbose フラグをセットする．
  virtual
  void
  set_verbose(int verbose);

  /// @brief verbose フラグを得る．
  int
  verbose() const;

  /// @brief dom_method を指定する．
  virtual
  void
  set_dom_method(ymuint dom_method);

  /// @brief get_dom_faults() のアルゴリズムを指定する．
  ymuint
  dom_method() const;


protected:
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
       vector<TpgFault*>& fault_list) = 0;

  /// @brief 対象の全故障数を返す．
  virtual
  ymuint
  fault_num() = 0;

  /// @brief 最初の故障を選ぶ．
  virtual
  TpgFault*
  get_first_fault() = 0;

  /// @brief 次に処理すべき故障を選ぶ．
  /// @param[in] fgmgr 故障グループを管理するオブジェクト
  /// @param[in] group_list 現在のグループリスト
  ///
  /// 故障が残っていなければ NULL を返す．
  virtual
  TpgFault*
  get_next_fault(FgMgr& fgmgr,
		 const vector<ymuint>& group_list) = 0;

  /// @brief 故障を追加するグループを選ぶ．
  /// @param[in] fgmgr 故障グループを管理するオブジェクト
  /// @param[in] fault 故障
  /// @param[in] group_list 現在のグループリスト
  ///
  /// グループが見つからなければ fgmgr.group_num() を返す．
  /// デフォルト実装では group_list のなかで最初にヒットしたものを返す．
  virtual
  ymuint
  find_group(FgMgr& fgmgr,
	     TpgFault* fault,
	     const vector<ymuint>& group_list);

  /// @brief テストパタンを作る．
  /// @param[in] network ネットワーク
  /// @param[in] suf_list 十分割当リスト
  /// @param[in] tv テストベクタ
  void
  make_testvector(TpgNetwork& network,
		  const NodeValList& suf_list,
		  TestVector* tv);

  /// @brief 故障解析器を返す．
  FaultAnalyzer&
  analyzer();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号の最大値＋１
  ymuint mMaxNodeId;

  // verbose フラグ
  int mVerbose;

  // group dominance フラグ
  bool mGroupDominance;

  // get_dom_fatuls() のアルゴリズム
  ymuint mDomMethod;

  // 故障解析器
  FaultAnalyzer mAnalyzer;

};

END_NAMESPACE_YM_SATPG

#endif // MINPATBASE_H
