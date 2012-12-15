#ifndef DTPGSAT_H
#define DTPGSAT_H

/// @file DtpgSat.h
/// @brief DtpgSat のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "DtpgNetwork.h"
#include "DtpgNode.h"
#include "ym_logic/Bool3.h"
#include "ym_logic/Literal.h"
#include "ym_logic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG

class FaultMgr;
class SaFault;
class TvMgr;
class TestVector;

END_NAMESPACE_YM_SATPG

BEGIN_NAMESPACE_YM_SATPG_DTPG

class DtpgFFR;

//////////////////////////////////////////////////////////////////////
/// @class DtpgSat DtpgSat.h "DtpgSat.h"
/// @brief SAT を用いたパタン生成器
//////////////////////////////////////////////////////////////////////
class DtpgSat
{
public:

  /// @brief コンストラクタ
  DtpgSat();

  /// @brief デストラクタ
  virtual
  ~DtpgSat();


public:
  //////////////////////////////////////////////////////////////////////
  // パタン生成を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL);

  /// @brief 回路と故障リストを設定する．
  /// @param[in] tgnetwork 対象のネットワーク
  /// @param[in] fault_list 故障リスト
  void
  set_network(const TgNetwork& tgnetwork,
	      const vector<SaFault*>& fault_list);

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  void
  dtpg_single(FaultMgr& fmgr,
	      TvMgr& tvmgr);

  /// @brief dual モードでテスト生成を行なう．
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  void
  dtpg_dual(FaultMgr& fmgr,
	    TvMgr& tvmgr);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  void
  dtpg_ffr(FaultMgr& fmgr,
	   TvMgr& tvmgr);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  void
  dtpg_mffc(FaultMgr& fmgr,
	    TvMgr& tvmgr);

  /// @brief all モードでテスト生成を行なう．
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  void
  dtpg_all(FaultMgr& fmgr,
	   TvMgr& tvmgr);

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  void
  dtpg_single_posplit(FaultMgr& fmgr,
		      TvMgr& tvmgr);

  /// @brief dual モードでテスト生成を行なう．
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  void
  dtpg_dual_posplit(FaultMgr& fmgr,
		    TvMgr& tvmgr);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  void
  dtpg_ffr_posplit(FaultMgr& fmgr,
		   TvMgr& tvmgr);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  void
  dtpg_mffc_posplit(FaultMgr& fmgr,
		    TvMgr& tvmgr);

  /// @brief all モードでテスト生成を行なう．
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  void
  dtpg_all_posplit(FaultMgr& fmgr,
		   TvMgr& tvmgr);

  /// @brief StatList をクリアする．
  void
  clear_stats_list();

  /// @brief StatList を得る．
  const vector<SatStats>&
  stats_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一つの故障に対してテストパタン生成を行なう．
  /// @param[in] f 故障
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  /// @param[out] u_list 検査不能故障を格納するリスト
  void
  single_mode(DtpgFault* f,
	      FaultMgr& fmgr,
	      TvMgr& tvmgr,
	      vector<DtpgFault*>& u_list);

  /// @brief 一つのノードに対して dual_mode でテストパタン生成を行なう．
  /// @param[in] node 対象のノード
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  /// @param[out] u_list 検査不能故障を格納するリスト
  void
  dual_mode_node(DtpgNode* node,
		 FaultMgr& fmgr,
		 TvMgr& tvmgr,
		 vector<DtpgFault*>& u_list);

  /// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
  /// @param[in] f0 0縮退故障
  /// @param[in] f1 1縮退故障
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  /// @param[out] u_list 検査不能故障を格納するリスト
  void
  dual_mode(DtpgFault* f0,
	    DtpgFault* f1,
	    FaultMgr& fmgr,
	    TvMgr& tvmgr,
	    vector<DtpgFault*>& u_list);

  /// @brief FFR 内の故障に対してテストパタン生成を行なう．
  /// @param[in] ffr FFR を表すクラス
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  /// @param[out] u_list 検査不能故障を格納するリスト
  void
  ffr_mode(DtpgFFR* ffr,
	   FaultMgr& fmgr,
	   TvMgr& tvmgr,
	   vector<DtpgFault*>& u_list);

  /// @brief fnode の故障が伝搬する条件を表す CNF を作る．
  /// @param[in] solver SAT ソルバ
  /// @param[in] fnode 対象のノード
  /// @param[out] input_list 関係する外部入力ノードを納めるリスト
  void
  make_prop_cnf(SatSolver& solver,
		DtpgNode* fnode,
		vector<DtpgNode*>& input_list);

  /// @brief 入力に故障を持つノードの CNF を作る．
  /// @param[in] solver SAT ソルバ
  /// @param[in] fnode 対象のノード
  /// @param[in] ipos 故障のある入力の番号
  void
  make_ifault_cnf(SatSolver& solver,
		  DtpgNode* fnode,
		  ymuint ipos);

  /// @brief ノードの入出力の関係を表す CNF を作る．
  /// @param[in] solver SAT ソルバ
  /// @param[in] node 対象のノード
  /// @param[in] olit 出力のリテラル
  /// @param[in] ilit 入力のリテラルの配列
  void
  make_node_cnf(SatSolver& solver,
		DtpgNode* node,
		Literal olit,
		const vector<Literal>& ilits);

  /// @brief SAT の結果からテストベクタを作る．
  void
  set_tv(const vector<Bool3>& model,
	 const vector<DtpgNode*>& input_list,
	 TestVector* tv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 直前の SAT の実行結果
  SatStats mStats;

  // SAT solver のタイプ
  string mType;

  // SAT solver のオプション
  string mOption;

  // SAT solver の記録用ストリーム
  ostream* mOutP;

  // 対象の回路
  DtpgNetwork* mNetwork;

  // SatStats のリスト
  vector<SatStats> mStatsList;

};

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGSAT_H
