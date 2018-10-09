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
/// @class EquivResult EquivMgr.h "EquivMgr.h"
/// @brief EquivMgr::check() の結果を表すクラス
//////////////////////////////////////////////////////////////////////
class EquivResult
{
public:

  /// @brief コンストラクタ
  /// @param[in] result 全体の結果
  /// @param[in] output_results 各出力ごとの結果のリスト
  EquivResult(SatBool3 result = SatBool3::X,
	      const vector<SatBool3>& output_results = vector<SatBool3>());

  /// @brief デストラクタ
  ~EquivResult();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 全体の結果を返す．
  /// @retval SatBool3::True 等価
  /// @retval SatBool3::False 非等価
  /// @retval SatBool3::X アボート(不明)
  SatBool3
  result() const;

  /// @brief 各出力ごとの結果のリストを返す．
  const vector<SatBool3>&
  output_results() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全体の結果
  SatBool3 mResult;

  // 各出力ごとの結果のリスト
  vector<SatBool3> mOutputResults;

};


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
  EquivMgr(int sig_size = 1,
	   const SatSolverType& solver_type = SatSolverType());


  /// @brief デストラクタ
  ~EquivMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 検証を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つの回路が等価かどうか調べる．
  /// @param[in] network1 対象の回路1
  /// @param[in] network2 対象の回路2
  /// @param[out] eq_stats 各出力ごとの等価検証の結果
  ///
  /// 入力と出力の対応関係は順序で対応させる．
  EquivResult
  check(const BnNetwork& network1,
	const BnNetwork& network2);

  /// @brief 2つの回路が等価かどうか調べる．
  /// @param[in] network1 対象の回路1
  /// @param[in] network2 対象の回路2
  /// @param[in] input_pair_list 入力ノード番号の対のリスト
  /// @param[in] output_pair_list 出力ノード番号の対のリスト
  EquivResult
  check(const BnNetwork& network1,
	const BnNetwork& network2,
	const vector<pair<int, int>>& input_pair_list,
	const vector<pair<int, int>>& output_pair_list);


public:
  //////////////////////////////////////////////////////////////////////
  // 制御パラーメタ関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シグネチャのサイズを設定する．
  /// @param[in] sig_size シグネチャのサイズ
  void
  set_signature_size(int sig_size);

  /// @brief SATソルバの種類を設定する．
  /// @param[in] solver_type SAT-solver の種類を表すオブジェクト
  void
  set_sat_solver_type(const SatSolverType& solver_type);

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


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] result 全体の結果
// @param[in] output_results 各出力ごとの結果のリスト
inline
EquivResult::EquivResult(SatBool3 result,
			 const vector<SatBool3>& output_results) :
  mResult(result),
  mOutputResults(output_results)
{
}

// @brief デストラクタ
inline
EquivResult::~EquivResult()
{
}

// @brief 全体の結果を返す．
inline
SatBool3
EquivResult::result() const
{
  return mResult;
}

// @brief 各出力ごとの結果のリストを返す．
inline
const vector<SatBool3>&
EquivResult::output_results() const
{
  return mOutputResults;
}

// @brief シグネチャのサイズを設定する．
// @param[in] sig_size シグネチャのサイズ
inline
void
EquivMgr::set_signature_size(int sig_size)
{
  mSigSize = sig_size;
}

// @brief SATソルバの種類を設定する．
// @param[in] solver_type SAT-solver の種類を表すオブジェクト
inline
void
EquivMgr::set_sat_solver_type(const SatSolverType& solver_type)
{
  mSolverType = solver_type;
}

// @brief ログレベルを設定する．
// @param[in] level 設定するログレベル
inline
void
EquivMgr::set_loglevel(int level)
{
  mLogLevel = level;
}

// @brief ログ出力用ストリームを設定する．
// @param[in] out ストリームのポインタ
inline
void
EquivMgr::set_logstream(ostream* out)
{
  mLogOutP = out;
}

// @brief ランダムシミュレーション制御用のパラメータを設定する．
// @param[in] loop_limit 変化のない状態がこの回数連続したら止める．
inline
void
EquivMgr::set_loop_limit(int loop_limit)
{
  mLoopLimit = loop_limit;
}

// @brief ログレベルを返す．
inline
int
EquivMgr::log_level() const
{
  return mLogLevel;
}

// @brief ログの出力先を返す．
inline
ostream&
EquivMgr::log_out() const
{
  return *mLogOutP;
}

END_NAMESPACE_MAGUS

#endif // EQUIVMGR_H
