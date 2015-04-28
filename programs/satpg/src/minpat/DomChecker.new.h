#ifndef DOMCHECKER_H
#define DOMCHECKER_H

/// @file DomChecker.h
/// @brief DomChecker のヘッダファイル
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
/// @class DomChecker DomChecker.h "DomChecker.h"
/// @brief 故障間の支配関係を解析するクラス
//////////////////////////////////////////////////////////////////////
class DomChecker
{
public:

  /// @brief コンストラクタ
  /// @param[in] analyzer 故障の情報を持つクラス
  /// @param[in] tvmgr テストベクタのマネージャ
  /// @param[in] fsim 故障シミュレータ
  DomChecker(FaultAnalyzer& analyzer,
	     TvMgr& tvmgr,
	     Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~DomChecker();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief verbose フラグを設定する．
  /// @param[in] verbose 表示を制御するフラグ
  void
  set_verbose(int verbose);

  /// @brief 支配故障を求める．
  ///
  /// 結果は mDomFaultList に格納される．
  void
  get_dom_faults(ymuint method,
		 const vector<const TpgFault*>& src_list,
		 vector<const TpgFault*>& dom_fault_list);

  /// @brief シミュレーション時の検出パタン数を返す．
  ymuint
  det_count(ymuint f_id);

  /// @brief 被支配故障候補数を返す．
  ymuint
  dom_cand_size(ymuint f_id);

  /// @brief 支配故障候補数を返す．
  ymuint
  dom_cand2_size(ymuint f_id,
		 ymuint idx);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 統計データ
  struct DomStats
  {
    void
    clear()
    {
      mSingleSat = 0;
      mNoDom = 0;
      mSingleDom = 0;
      mSat = 0;
      mDom = 0;
    }

    ymuint mSingleSat;

    ymuint mNoDom;

    ymuint mSingleDom;

    ymuint mSat;

    ymuint mDom;
  };

  // 故障ごとのデータ
  struct FaultData
  {

    // 被支配故障候補のリスト
    vector<ymuint> mDomCandList1;

    // 支配故障候補のリスト
    vector<ymuint> mDomCandList2[3];

    // 検出パタン数
    ymuint mDetCount;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
  /// @param[in] fault_list 故障リスト
  void
  do_fsim(const vector<const TpgFault*>& fault_list);

  /// @brief 故障シミュレーションの後処理
  ymuint
  get_dom_cand(const vector<pair<ymuint, PackedVal> >& det_list);

  /// @brief 各故障ごとの候補リストを作る．
  /// @param[in] fault_list 故障リスト
  void
  make_cand_list(const vector<const TpgFault*>& fault_list);

  void
  drop_dom_faults(const vector<ymuint>& fault_list,
		  ymuint idx,
		  vector<bool>& dom_flag,
		  vector<ymuint>& dst_fault_list);

  /// @brief f1_id を支配する故障を fault_list の中から探す．
  ///
  /// 支配されていたら true を返す．
  bool
  check_faults_dominance(ymuint f1_id,
			 const vector<ymuint>& fault_list,
			 const vector<bool>& dom_flag,
			 DomStats& dom_stats);

  /// @brief f1 が f2 を支配しているか調べる．
  bool
  check_fault_dominance(const TpgFault* f1,
			const TpgFault* f2);

  /// @brief DomStats を出力する．
  static
  void
  print_stats(ostream& s,
	      const DomStats& stats);


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

  // get_dom_cand() 中で用いる配列
  vector<PackedVal> mDetFlag;

  USTime mSuccessTime;

  USTime mSuccessMax;

  USTime mFailureTime;

  USTime mFailureMax;

  USTime mAbortTime;

  USTime mAbortMax;

  ymuint mDomCheckCount;

  ymuint mSingleNum;

  DomStats mStats[3];

  ymuint mPat;
};

END_NAMESPACE_YM_SATPG

#endif // DOMCHECKER_H
