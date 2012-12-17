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
#include "ym_networks/tgnet.h"
#include "ym_logic/SatStats.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

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
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  single(DtpgOperator& op);

  /// @brief dual モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  dual(DtpgOperator& op);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  ffr(DtpgOperator& op);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  mffc(DtpgOperator& op);

  /// @brief all モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  all(DtpgOperator& op);

  /// @brief single モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  single_posplit(DtpgOperator& op);

  /// @brief dual モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  dual_posplit(DtpgOperator& op);

  /// @brief ffr モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  ffr_posplit(DtpgOperator& op);

  /// @brief mffc モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  mffc_posplit(DtpgOperator& op);

  /// @brief all モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  void
  all_posplit(DtpgOperator& op);

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

  /// @brief 一つのノードに対して dual_mode でテストパタン生成を行なう．
  /// @param[in] node 対象のノード
  /// @param[in] fmgr 故障の管理用オブジェクト
  /// @param[in] tvmgr テストベクタの管理用オブジェクト
  /// @param[out] u_list 検査不能故障を格納するリスト
  void
  dual_mode_node(DtpgNode* node,
		 DtpgOperator& op);

  /// @brief 一つの FFR に対してテストパタン生成を行う．
  /// @param[in] ffr 対象の FFR
  void
  ffr_mode(DtpgFFR* ffr,
	   DtpgOperator& op);


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
