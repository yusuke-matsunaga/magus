#ifndef DTPGSATS3_H
#define DTPGSATS3_H

/// @file DtpgSatS3.h
/// @brief DtpgSatS3 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"
#include "NodeValList.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSatS3 DtpgSatS3.h "DtpgSatS3.h"
/// @brief 1つの故障を対象とした CNF を生成する DtpgSat
//////////////////////////////////////////////////////////////////////
class DtpgSatS3 :
  public DtpgSat
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  DtpgSatS3(const string& sat_type,
	    const string& sat_option,
	    ostream* sat_outp,
	    BackTracer& bt,
	    DetectOp& dop,
	    UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~DtpgSatS3();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] network 対象のネットワーク
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& network,
      DtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] network 対象のネットワーク
  /// @param[in] f_tgt 対象の故障
  bool
  run_single(TpgNetwork& network,
	     TpgFault* f_tgt);

  /// @brief 他の故障との関係を調べる．
  void
  check_other_faults(TpgNetwork& network,
		     TpgFault* f_tgt,
		     const vector<TpgFault*>& fault_list);


private:
  //////////////////////////////////////////////////////////////////////
  // このクラスで使われるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief 各故障ごとの情報をまとめた構造体
  struct FaultInfo
  {
    /// @brief コンストラクタ
    FaultInfo();

    /// @brief デストラクタ
    ~FaultInfo();

    // 検出可能フラグ
    bool mDetected;

    // 十分割当リスト
    NodeValList mSufList;

    // 必要割当リスト
    NodeValList mMaList;

    // 排他的な故障のリスト
    vector<ymuint> mConflictList;

    // 支配している故障のリスト
    vector<ymuint> mDominateList;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各故障の情報を収める配列
  vector<FaultInfo> mFaultInfoArray;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGSATS3_H
