#ifndef DTPGSATC_H
#define DTPGSATC_H

/// @file DtpgSatC.h
/// @brief DtpgSatC のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSatC DtpgSatC.h "DtpgSatC.h"
/// @brief 複数の故障の検出に使える CNF 式を生成するタイプの SatEngine
//////////////////////////////////////////////////////////////////////
class DtpgSatC :
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
  DtpgSatC(const string& sat_type,
	   const string& sat_option,
	   ostream* sat_outp,
	   BackTracer& bt,
	   DetectOp& dop,
	   UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~DtpgSatC();


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
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DFS で MFFC を求める．
  /// @param[in] node 対象のノード
  void
  dfs_mffc(TpgNode* node);

  /// @brief 複数の故障を同時に検出するパタンを求める．
  void
  run_multi(NodeSet& node_set,
	    TpgNode* root);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障を持つノードのリスト
  vector<TpgNode*> mFaultNodeList;

  // 対象の故障リスト
  vector<TpgFault*> mFaultList;

  // 作業用のノードリスト
  vector<TpgNode*> mTmpNodeList;

  // 作業用のマーク
  vector<bool> mMark;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGSATC_H
