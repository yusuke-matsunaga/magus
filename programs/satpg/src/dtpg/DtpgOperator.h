#ifndef DTPGOPERATOR_H
#define DTPGOPERATOR_H

/// @file DtpgOperator.h
/// @brief DtpgOperator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgOperator DtpgOperator.h "DtpgOperator.h"
/// @brief DtpgSat の処理後に呼ばれるファンクター
//////////////////////////////////////////////////////////////////////
class DtpgOperator
{
public:

  /// @brief テストパタンが見つかった場合に呼ばれる関数
  virtual
  void
  set_detected(DtpgFault* f,
	       const vector<DtpgNode*>& input_list,
	       const vector<Bool3>& model) = 0;

  /// @brief 検出不能のときに呼ばれる関数
  virtual
  void
  set_untestable(DtpgFault* f) = 0;

};

/// @brief 一つの故障に対してテストパタン生成を行なう．
/// @param[in] solver SatSolver
/// @@aram[in] network 対象のネットワーク
/// @param[in] f 故障
/// @param[in] op テスト生成の結果を処理するファンクター
extern
void
dtpg_single(SatSolver& solver,
	    DtpgNetwork& network,
	    DtpgFault* f,
	    DtpgOperator& op);

/// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
/// @param[in] solver SatSolver
/// @@aram[in] network 対象のネットワーク
/// @param[in] f0 0縮退故障
/// @param[in] f1 1縮退故障
/// @param[in] op テスト生成の結果を処理するファンクター
extern
void
dtpg_dual(SatSolver& solver,
	  DtpgNetwork& network,
	  DtpgFault* f0,
	  DtpgFault* f1,
	  DtpgOperator& op);

/// @brief FFR 内の故障に対してテストパタン生成を行なう．
/// @param[in] solver SatSolver
/// @@aram[in] network 対象のネットワーク
/// @param[in] flist 故障リスト
/// @param[in] root FFR の根のノード
/// @param[in] node_list FFR 内のノードリスト
/// @param[in] op テスト生成の結果を処理するファンクター
extern
void
dtpg_ffr(SatSolver& solver,
	 DtpgNetwork& network,
	 const vector<DtpgFault*>& flist,
	 DtpgNode* root,
	 const vector<DtpgNode*>& node_list,
	 DtpgOperator& op);

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGOPERATOR_H
