#ifndef DOMCHECKER_H
#define DOMCHECKER_H

/// @file DomChecker.h
/// @brief DomChecker のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
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

  /// @brief 非支配故障候補数を返す．
  ymuint
  dom_cand_size(ymuint f_id);

  /// @brief 支配故障候補数を返す．
  ymuint
  dom_cand2_size(ymuint f_id);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 支配故障を求める．
  ///
  /// 結果は mDomFaultList に格納される．
  void
  get_dom_faults1(const vector<const TpgFault*>& src_list,
		  vector<const TpgFault*>& dom_fault_list);

  /// @brief 支配故障を求める．
  ///
  /// 結果は mDomFaultList に格納される．
  void
  get_dom_faults2(ymuint option ,
		  const vector<const TpgFault*>& src_list,
		  vector<const TpgFault*>& dom_fault_list);

  /// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
  /// @param[in] fault_list 故障リスト
  void
  get_pat_list(const vector<const TpgFault*>& fault_list);

  /// @brief 故障シミュレーションの後処理
  ymuint
  record_pat(const vector<ymuint>& det_list,
	     ymuint pat_id);

  /// @brief f1 が f2 を支配しているか調べる．
  bool
  check_fault_dominance(const TpgFault* f1,
			const TpgFault* f2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 故障ごとのデータ
  struct FaultData
  {

    // 被支配故障候補のリスト
    vector<ymuint> mDomCandList;

    // mDomCandList の本当の要素数
    ymuint mDomCandListSize;

    // 支配故障候補のリスト
    vector<ymuint> mDomCandList2;

    // 検出パタン数
    ymuint mDetCount;

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

  // record_pat() 中で用いる配列
  vector<bool> mDetFlag;

};

END_NAMESPACE_YM_SATPG

#endif // DOMCHECKER_H
