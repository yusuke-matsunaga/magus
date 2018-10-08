#ifndef EQUIVMGR_H
#define EQUIVMGR_H

/// @file EquivMgr.h
/// @brief EquivMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "magus.h"
#include "ym/bnet.h"
#include "ym/SatBool3.h"
#include "ym/SatSolverType.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class EquivMgr EquivMgr.h "EquivMgr.h"
/// @brief combinational equivalennce check を行うクラス
//////////////////////////////////////////////////////////////////////
class EquivMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] sig_size シグネチャのサイズ
  /// @param[in] solver_type SAT-solver の種類を表すオブジェクト
  EquivMgr(int sig_size,
	   const SatSolverType& solver_type = SatSolverType());


  /// @brief デストラクタ
  ~EquivMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 検証を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つの回路が等価かどうか調べる．
  /// @param[in] network1 対象の回路1
  /// @param[in] input1_list 入力ノード番号のリスト
  /// @param[in] output1_list 出力ノード番号のリスト
  /// @param[in] network2 対象の回路2
  /// @param[in] input2_list 入力ノード番号のリスト
  /// @param[in] output2_list 出力ノード番号のリスト
  /// @param[out] eq_stats 各出力ごとの等価検証の結果
  /// @retval SatBool3::True 等価
  /// @retval SatBool3::False 非等価
  /// @retval SatBool3::X アボート(不明)
  SatBool3
  check(const BnNetwork& network1,
	const vector<int>& input1_list,
	const vector<int>& output1_list,
	const BnNetwork& network2,
	const vector<int>& input2_list,
	const vector<int>& output2_list,
	vector<SatBool3>& eq_stats);


public:
  //////////////////////////////////////////////////////////////////////
  // 制御パラーメタ関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ログレベルを設定する．
  /// @param[in] level 設定するログレベル
  void
  set_loglevel(int level);

  /// @brief ログ出力用ストリームを設定する．
  /// @param[in] out ストリームのポインタ
  void
  set_logstream(ostream* out);

  /// @brief ランダムシミュレーション制御用のパラメータを設定する．
  /// @param[in] loop_limit 変化のない状態がこの回数連続したら止める．
  void
  set_loop_limit(int loop_limit);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ログレベルを返す．
  int
  log_level() const;

  /// @brief ログの出力先を返す．
  ostream&
  log_out() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // シグネチャのサイズ
  int mSigSize;

  // SATソルバのタイプ
  SatSolverType mSolverType;

  // ログレベル
  int mLogLevel;

  // ログの出力先
  ostream* mLogOutP;

  // ランダムシミュレーション制御用のパラメータ
  int mLoopLimit;

};

END_NAMESPACE_MAGUS

#endif // EQUIVMGR_H
