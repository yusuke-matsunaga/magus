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

#define USE_CANDLIST1 1


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
  /// @param[in] src_fid_list 入力の故障番号リスト
  /// @param[out] dom_fid_list 支配故障番号リスト
  void
  get_dom_faults(const vector<ymuint>& src_fid_list,
		 vector<ymuint>& dom_fid_list);

  /// @brief シミュレーション時の検出パタン数を返す．
  ymuint
  det_count(ymuint f_id);

  /// @brief single cube condition かどうかを返す．
  bool
  single_cube(ymuint f_id);

  /// @brief 非支配故障候補数を返す．
  ymuint
  dom_cand_size(ymuint f_id);

  /// @brief 支配故障候補数を返す．
  ymuint
  dom_cand2_size(ymuint f_id);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 故障ごとのデータ
  struct FaultData
  {

    // 被支配故障候補のリスト
    vector<ymuint> mDomCandList1;

    // 支配故障候補のリスト
    vector<ymuint> mDomCandList2[2];

    // 検出パタン数
    ymuint mDetCount;

  };

  // 統計データ
  struct DomStats
  {
    void
    clear();

    void
    print(ostream& s) const;

    const DomStats&
    operator+=(const DomStats& right);

    ymuint mSingleSat;

    ymuint mNoDom;

    ymuint mSingleDom;

    ymuint mSat;

    ymuint mDom;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障シミュレーションを行い，支配故障の候補リストを作る．
  /// @param[in] fid_list 故障リスト
  void
  do_fsim(const vector<ymuint>& fid_list);

  /// @brief do_fsim() の下請け処理
  ymuint
  record_dom_cand(const vector<pair<ymuint, PackedVal> >& det_list);

  /// @brief 支配故障を求める基本処理
  /// @param[in] src_fid_list 対象の故障のリスト
  /// @param[in] idx 故障候補リストのインデックス
  /// @param[in] dst_fid_list 支配されていない故障のリスト
  void
  get_dom_faults1(const vector<ymuint>& src_fid_list,
		  ymuint idx,
		  vector<ymuint>& dst_fid_list);


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

  // 支配されたことを示すフラグ
  vector<bool> mDomFlag;

  USTime mSuccessTime;

  USTime mSuccessMax;

  USTime mFailureTime;

  USTime mFailureMax;

  USTime mAbortTime;

  USTime mAbortMax;

  ymuint mDomCheckCount;

  DomStats mStats[3];

  ymuint mPat;

  ymuint mSmartDomCheck;

};

END_NAMESPACE_YM_SATPG

#endif // DOMCHECKER_H
