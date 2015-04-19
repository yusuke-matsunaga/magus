#ifndef FAULTANALYZER_H
#define FAULTANALYZER_H

/// @file FaultAnalyzer.h
/// @brief FaultAnalyzer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "NodeValList.h"
#include "FaultInfo.h"
#include "NodeSet.h"
#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FaultAnalyzer FaultAnalyzer.h "FaultAnalyzer.h"
/// @brief 故障間の関係を解析するクラス
//////////////////////////////////////////////////////////////////////
class FaultAnalyzer
{
public:

  /// @brief コンストラクタ
  FaultAnalyzer();

  /// @brief デストラクタ
  virtual
  ~FaultAnalyzer();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief verbose フラグを設定する．
  /// @param[in] verbose 表示を制御するフラグ
  void
  set_verbose(int verbose);

  /// @brief verbose フラグを得る．
  int
  verbose() const;

  /// @brief 初期化する．
  /// @param[in] network ネットワーク
  /// @param[in] tvmgr テストベクタのマネージャ
  /// @param[out] fault_list 検出された故障のリスト
  void
  init(const TpgNetwork& network,
       TvMgr& tvmgr,
       vector<TpgFault*>& fault_list);

  /// @brief ノード番号の最大値を得る．
  ymuint
  max_node_id() const;

  /// @brief 故障番号の最大値を得る．
  ymuint
  max_fault_id() const;

  /// @brief 検出可能な故障のリストを得る．
  const vector<TpgFault*>&
  fault_list() const;

  /// @brief 故障を得る．
  /// @param[in] fid 故障番号
  TpgFault*
  fault(ymuint fid);

  /// @brief 個別の故障の情報を得る．
  /// @param[in] fid 故障番号
  const FaultInfo&
  fault_info(ymuint fid) const;

  /// @brief 故障のTFOのTFIに含まれる入力番号のリスト返す．
  /// @param[in] fid 故障番号
  const vector<ymuint>&
  input_list(ymuint fid);

  /// @brief 故障のTFIに含まれる入力番号のリスト返す．
  /// @param[in] fid 故障番号
  const vector<ymuint>&
  input_list2(ymuint fid);

  /// @brief 故障に関連するノード集合を返す．
  const NodeSet&
  node_set(ymuint fid);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障の解析を行う．
  /// @param[in] fault 故障
  /// @param[in] tvmgr テストベクタのマネージャ
  Bool3
  analyze_fault(TpgFault* fault,
		TvMgr& tvmgr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 表示を制御するフラグ
  int mVerbose;

  // 最大ノード番号
  ymuint mMaxNodeId;

  // 最大故障番号
  ymuint mMaxFaultId;

  // テストベクタ用の乱数生成器
  RandGen mRandGen;

  // オリジナルの故障リスト
  vector<TpgFault*> mOrigFaultList;

  // 検出可能故障用のテストベクタリスト
  vector<TestVector*> mTestVectorList;

  // ノードごとに関係する入力の番号のリストを収める配列
  vector<vector<ymuint> > mInputListArray;

  // ノードごとに関係する入力の番号のリストを収める配列
  vector<vector<ymuint> > mInputList2Array;

  // ノードごとに NodeSet を収める配列
  vector<NodeSet> mNodeSetArray;

  // 故障ごとの情報を収める配列
  vector<FaultInfo> mFaultInfoArray;

};

END_NAMESPACE_YM_SATPG

#endif // FAULTANALYZER_H
