#ifndef CONFLICTCHECKER_H
#define CONFLICTCHECKER_H

/// @file ConflictChecker.h
/// @brief ConflictChecker のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "PackedVal.h"
#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

class FaultAnalyzer;

//////////////////////////////////////////////////////////////////////
/// @class ConflictChecker ConflictChecker.h "ConflictChecker.h"
/// @brief 故障間の支配関係を解析するクラス
//////////////////////////////////////////////////////////////////////
class ConflictChecker
{
public:

  /// @brief コンストラクタ
  /// @param[in] analyzer 故障の情報を持つクラス
  ConflictChecker(FaultAnalyzer& analyzer,
		  TvMgr& tvmgr,
		  Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~ConflictChecker();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief verbose フラグを設定する．
  /// @param[in] verbose 表示を制御するフラグ
  void
  set_verbose(int verbose);

  /// @brief 衝突の解析を行う．
  void
  analyze_conflict(const vector<const TpgFault*>& fault_list);

  /// @brief 衝突リストを得る．
  ///
  /// 事前に analyze_conflict() を実行しておく必要がある．
  const vector<ymuint>&
  conflict_list(ymuint fid);

  /// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
  /// @param[in] fault_list 故障リスト
  void
  do_fsim(const vector<const TpgFault*>& fault_list);

  /// @brief 1つの故障に対する衝突の解析を行う．
  ///
  /// 事前に do_fsim() を実行しておく必要がある．
  void
  analyze_conflict(const TpgFault* f1,
		   const vector<const TpgFault*>& fault_list,
		   vector<ymuint>& conf_list);

  /// @brief 衝突数の見積もりを行う．
  void
  estimate_conflict(const vector<const TpgFault*>& fault_list,
		    ymuint sample_num,
		    vector<double>& conf_prob_array);

  /// @brief 衝突数の見積もりを行う．
  ///
  /// 事前に do_fsim() を実行しておく必要がある．
  void
  estimate_conflict(const vector<const TpgFault*>& fault_list,
		    vector<ymuint>& conf_num_array);


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
  analyze_conflict(const TpgFault* f1,
		   const vector<ymuint>& f2_list,
		   vector<ymuint>& conf_list,
		   bool simple,
		   bool local_verbose);

  /// @brief 1つの故障と複数の故障間の衝突性を調べる．
  /// @param[in] f1 対象の故障
  /// @param[in] f2_list f1 との衝突性を調べる故障のリスト
  /// @param[out] f1 と衝突する故障のリスト
  /// @param[in] simple 高速化ヒューリスティック
  /// @param[in] local_verbose 出力制御フラグ
  void
  analyze_conflict2(const TpgFault* f1,
		    const vector<ymuint>& f2_list,
		    vector<ymuint>& conf_list,
		    bool simple,
		    bool local_verbose);

  /// @brief 故障シミュレーションの後処理
  ymuint
  record_pat(const vector<pair<ymuint, PackedVal> >& det_list,
	     const vector<const TpgFault*>& fault_list);

  /// @brief f1 が f2 を支配しているか調べる．
  bool
  check_fault_conflict(const TpgFault* f1,
		       const TpgFault* f2);

  /// @brief analyze_conflict の統計情報を出力する．
  void
  print_conflict_stats(ostream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 故障ごとのデータ
  struct FaultData
  {
    ymuint mDetCount;

    // 衝突候補の故障番号リスト
    vector<ymuint> mCandList;

    ymuint mCandListSize;

    vector<ymuint> mMaConflictList;

    // 衝突している故障のリスト
    vector<ymuint> mConflictList;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 表示を制御するフラグ
  int mVerbose;

  // 故障の情報を持つクラス
  FaultAnalyzer& mAnalyzer;

  // テストベクタを管理するクラス
  TvMgr& mTvMgr;

  // 故障シミュレータ
  Fsim& mFsim;

  // 最大ノード番号
  ymuint mMaxNodeId;

  // 最大故障番号
  ymuint mMaxFaultId;

  // テストベクタ用の乱数生成器
  RandGen mRandGen;

  // 故障ごとのデータ配列
  vector<FaultData> mFaultDataArray;

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

#endif // CONFLICTCHECKER_H
