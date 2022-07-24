#ifndef EQUIVMGR_H
#define EQUIVMGR_H

/// @file EquivMgr.h
/// @brief EquivMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
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
  EquivResult(
    SatBool3 result = SatBool3::X,              ///< [in] 全体の結果
    const vector<SatBool3>& output_results = {} ///< [in] 各出力ごとの結果のリスト
  ) : mResult{result},
      mOutputResults{output_results}
  {
  }

  /// @brief デストラクタ
  ~EquivResult() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 全体の結果を返す．
  /// @retval SatBool3::True 等価
  /// @retval SatBool3::False 非等価
  /// @retval SatBool3::X アボート(不明)
  SatBool3
  result() const
  {
    return mResult;
  }

  /// @brief 各出力ごとの結果のリストを返す．
  const vector<SatBool3>&
  output_results() const
  {
    return mOutputResults;
  }


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
  EquivMgr(
    SizeType sig_size = 1,           ///< [in] シグネチャのサイズ
    const SatSolverType& solver_type ///< [in] SAT-solver の種類を表すオブジェクト
    = SatSolverType{}
  );

  /// @brief デストラクタ
  ~EquivMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 検証を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つの回路が等価かどうか調べる．
  ///
  /// デフォルトでは入力と出力の対応関係は順序で対応させる．
  EquivResult
  check(
    const BnNetwork& network1, ///< [in] 対象の回路1
    const BnNetwork& network2, ///< [in] 対象の回路2
    bool match_by_name = false ///< [in] 対応関係を名前で取る．
  );

  /// @brief 2つの回路が等価かどうか調べる．
  ///
  /// input2_list, output2_list ともに，network2 の i 番目の
  /// 入力(出力)が network1 の何番目の入力(出力)に対応しているか
  /// を示す．
  EquivResult
  check(
    const BnNetwork& network1,      ///< [in] 対象の回路1
    const BnNetwork& network2,      ///< [in] 対象の回路2
    const vector<int>& input2_list, ///< [in] network2の入力順序を表すリスト
    const vector<int>& output2_list ///< [in] network2の出力順序を表すリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 制御パラーメタ関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シグネチャのサイズを設定する．
  void
  set_signature_size(
    SizeType sig_size ///< [in] シグネチャのサイズ
  )
  {
    mSigSize = sig_size;
  }

  /// @brief SATソルバの種類を設定する．
  void
  set_sat_solver_type(
    const SatSolverType& solver_type ///< [in] SAT-solver の種類を表すオブジェクト
  )
  {
    mSolverType = solver_type;
  }

  /// @brief ログレベルを設定する．
  void
  set_loglevel(
    int level ///< [in] 設定するログレベル
  )
  {
    mLogLevel = level;
  }

  /// @brief ログ出力用ストリームを設定する．
  void
  set_logstream(
    ostream* out ///< [in] ストリームのポインタ
  )
  {
    mLogOutP = out;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ログレベルを返す．
  int
  log_level() const
  {
    return mLogLevel;
  }

  /// @brief ログの出力先を返す．
  ostream&
  log_out() const
  {
    return *mLogOutP;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // シグネチャのサイズ
  SizeType mSigSize;

  // SATソルバのタイプ
  SatSolverType mSolverType;

  // ログレベル
  int mLogLevel{0};

  // ログの出力先
  ostream* mLogOutP{nullptr};

};

END_NAMESPACE_MAGUS

#endif // EQUIVMGR_H
