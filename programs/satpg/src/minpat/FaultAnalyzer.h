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
#include "YmUtils/StopWatch.h"
#include "YmUtils/RandGen.h"


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

  /// @brief 初期化する．
  /// @param[in] max_node_id ノード番号の最大値 + 1
  /// @param[in] fault_list 故障リスト
  void
  init(ymuint max_node_id,
       const vector<TpgFault*>& fault_list);

  /// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] tvmgr テストベクタのマネージャ
  /// @param[in] tv_list テストベクタのリスト
  /// @param[in] rg 乱数生成器
  ///
  /// 結果は mFaultInfoArray の pat_list に格納される．
  void
  get_pat_list(Fsim& fsim,
	       TvMgr& tvmgr,
	       const vector<TestVector*>& tv_list,
	       RandGen& rg);

  /// @brief 支配故障を求める．
  /// @param[in] fast 手抜きの高速化を行うとき true にするフラグ
  ///
  /// 結果は mDomFaultList に格納される．
  void
  get_dom_faults(bool fast);

  /// @brief 十分割当と必要割当を求める．
  ///
  /// 結果は mFaultInfoArray に格納される．
  void
  analyze_faults();

  /// @brief 故障間の衝突性を調べる．
  void
  analyze_conflict();

  /// @brief 故障間の衝突性を調べる．
  void
  estimate_conflict(ymuint sample_num,
		    vector<double>& conf_prob_array);

  /// @brief 1つの故障と複数の故障間の衝突性を調べる．
  /// @param[in] f1 対象の故障
  /// @param[in] f2_list f1 との衝突性を調べる故障のリスト
  /// @param[out] f1 と衝突する故障のリスト
  /// @param[in] simple 高速化ヒューリスティック
  /// @param[in] local_verbose 出力制御フラグ
  void
  analyze_conflict(TpgFault* f1,
		   const vector<TpgFault*>& f2_list,
		   vector<TpgFault*>& conf_list,
		   bool simple,
		   bool local_verbose);

  /// @brief 衝突リストを得る．
  void
  get_conf_list(vector<pair<ymuint, ymuint> >& conf_list);

  /// @brief 支配故障リストを得る．
  const vector<TpgFault*>&
  dom_fault_list() const;

  /// @brief 故障の情報を得る．
  const vector<FaultInfo>&
  fault_info_array() const;

  /// @brief 個別の故障の情報を得る．
  /// @param[in] fid 故障番号
  const FaultInfo&
  fault_info(ymuint fid) const;

  /// @brief 入力番号リストを得る．
  const vector<vector<ymuint> >&
  input_list_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1つの故障と複数の故障間の衝突性を調べる．
  /// @param[in] f1 対象の故障
  /// @param[in] f2_list f1 との衝突性を調べる故障のリスト
  /// @param[out] f1 と衝突する故障のリスト
  /// @param[in] simple 高速化ヒューリスティック
  /// @param[in] local_verbose 出力制御フラグ
  void
  analyze_conflict2(TpgFault* f1,
		    const vector<TpgFault*>& f2_list,
		    vector<TpgFault*>& conf_list,
		    bool simple,
		    bool local_verbose);

  /// @brief 故障シミュレーションの後処理
  ymuint
  record_pat(const vector<ymuint>& det_list,
	     ymuint pat_id);

  /// @brief f1 が f2 を支配しているか調べる．
  bool
  check_fault_dominance(TpgFault* f1,
			TpgFault* f2);

  /// @brief f1 と f2 が衝突しているか調べる．
  bool
  check_fault_conflict(TpgFault* f1,
		       TpgFault* f2);

  /// @brief analyze_conflict の統計情報を出力する．
  void
  print_conflict_stats(ostream& s);


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

  // オリジナルの故障リスト
  vector<TpgFault*> mOrigFaultList;

  // パタンごとの検出故障リスト
  vector<vector<ymuint> > mDetListArray;

  // 支配故障リスト
  vector<TpgFault*> mDomFaultList;

  // ノードごとに関係する入力の番号のリストを収める配列
  vector<vector<ymuint> > mInputListArray;

  // ノードごとに関係する入力の番号のリストを収める配列
  vector<vector<ymuint> > mInputList2Array;

  // ノードごとに NodeSet を収める配列
  vector<NodeSet> mNodeSetArray;

  // 故障ごとの情報を収める配列
  vector<FaultInfo> mFaultInfoArray;

  // analyze_conflict 用の統計情報
  struct {
    // コンフリクト回数
    ymuint conf_count;
    ymuint conf1_count;
    ymuint conf2_count;
    ymuint conf3_count;
    ymuint conf4_count;
    ymuint conf4_check_count;
    ymuint int1_count;
    ymuint int2_count;

    StopWatch conf_timer;
    StopWatch conf1_timer;
    StopWatch conf2_timer;
    StopWatch conf3_timer;
    StopWatch conf4_timer;
    StopWatch int1_timer;
    StopWatch int2_timer;
  } mConflictStats;
};

END_NAMESPACE_YM_SATPG

#endif // FAULTANALYZER_H
