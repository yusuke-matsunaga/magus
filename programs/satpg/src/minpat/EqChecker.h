#ifndef EQCHECKER_H
#define EQCHECKER_H

/// @file EqChecker.h
/// @brief EqChecker のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "EqSet.h"
#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

class FaultAnalyzer;

//////////////////////////////////////////////////////////////////////
/// @class EqChecker EqChecker.h "EqChecker.h"
/// @brief 故障間の支配関係を解析するクラス
//////////////////////////////////////////////////////////////////////
class EqChecker
{
public:

  /// @brief コンストラクタ
  /// @param[in] analyzer 故障の情報を持つクラス
  /// @param[in] tvmgr テストベクタのマネージャ
  /// @param[in] fsim 故障シミュレータ
  EqChecker(FaultAnalyzer& analyzer,
	    TvMgr& tvmgr,
	    Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~EqChecker();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief verbose フラグを設定する．
  /// @param[in] verbose 表示を制御するフラグ
  void
  set_verbose(int verbose);

  /// @brief 代表故障を求める．
  ///
  /// 結果は rep_fault_list に格納される．
  void
  get_rep_faults(const vector<const TpgFault*>& src_list,
		 vector<const TpgFault*>& rep_fault_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障シミュレーションを行い，等価故障の候補リストを作る．
  /// @param[in] fault_list 故障リスト
  void
  do_fsim(const vector<const TpgFault*>& fault_list);

  /// @brief f1 と f2 が等価かどうか調べる．
  bool
  check_fault_equivalence(const TpgFault* f1,
			  const TpgFault* f2);

  /// @brief f1 が f2 を支配しているか調べる．
  bool
  check_fault_dominance2(const TpgFault* f1,
			 const TpgFault* f2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////


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

  // 等価故障の候補リストを表すクラス
  EqSet mEqSet;

  USTime mSuccessTime;

  USTime mSuccessMax;

  USTime mFailureTime;

  USTime mFailureMax;

  USTime mAbortTime;

  USTime mAbortMax;

  ymuint mDomCheckCount;

  ymuint mPat;
};

END_NAMESPACE_YM_SATPG

#endif // EQCHECKER_H
