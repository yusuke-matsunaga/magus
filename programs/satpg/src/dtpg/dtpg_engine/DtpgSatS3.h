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
#include "ConflictSet.h"


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


public:
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

    // ID番号
    ymuint mId;

    // 対応する故障
    TpgFault* mFault;

    // 検出フラグ
    bool mDetected;

    // 支配フラグ
    bool mDominated;

    // 関係する入力番号のリスト
    vector<ymuint> mInputList;

    // 十分割当リスト
    NodeValList mSufList;

    // 必要割当リスト
    NodeValList mMaList;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] node_set ノード集合
  /// @param[in] f_tgt 対象の故障
  bool
  run_single(const NodeSet& node_set,
	     TpgFault* f_tgt);

  /// @brief 他の故障との関係を調べる．
  void
  check_other_faults(TpgNetwork& network,
		     const NodeSet& node_set,
		     ConflictSet& conflict_set,
		     FaultInfo* f_tgt,
		     const vector<FaultInfo*>& fault_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各故障の情報を収める配列
  vector<FaultInfo> mFaultInfoArray;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGSATS3_H
