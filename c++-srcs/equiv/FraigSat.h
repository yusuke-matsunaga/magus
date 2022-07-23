#ifndef FRAIGSAT_H
#define FRAIGSAT_H

/// @file FraigSat.h
/// @brief FraigSat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "fraig_nsdef.h"
#include "FraigHandle.h"
#include "StructTable.h"
#include "PatTable.h"
#include "ym/Expr.h"
#include "ym/SatBool3.h"
#include "ym/SatSolverType.h"
#include "ym/SatSolver.h"
#include "ym/SatModel.h"
#include <random>


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class FraigSat FraigSat.h "FraigSat.h"
/// @brief FraigNode に関する SAT Solver のクラス
//////////////////////////////////////////////////////////////////////
class FraigSat
{
public:

  /// @brief コンストラクタ
  FraigSat(
    const SatSolverType& solver_type ///< [in] SAT-solver の種類を表すオブジェクト
    = SatSolverType()
  );

  /// @brief デストラクタ
  ~FraigSat();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 新しい変数を作る．
  SatLiteral
  new_var();

  /// @brief ノードの入出力の関係を表す CNF 式を作る．
  void
  make_cnf(
    FraigNode* node ///< [in] 対象のノード
  );

  /// @brief 2つのノードが等価かどうか調べる．
  SatBool3
  check_equiv(
    FraigNode* node1, ///< [in] ノード1
    FraigNode* node2, ///< [in] ノード2
    bool inv          ///< [in] 極性の反転フラグ
  );

  /// @brief ノードが定数と等価かどうか調べる．
  SatBool3
  check_const(
    FraigNode* node, ///< [in] 対象のノード
    bool inv         ///< [in] false で 0, true で 1 を表す．
  );

#if 0
  /// @brief 2つのハンドルが等価かどうか調べる．
  SatBool3
  check_equiv(
    FraigHandle aig1, ///< [in] 入力1のハンドル
    FraigHandle aig2  ///< [in] 入力2のハンドル
  );
#endif

  /// @brief 直前のSATの結果を得る．
  const SatModel&
  model()
  {
    return mSolver.model();
  }

  /// @brief ログレベルを設定する．
  void
  set_loglevel(
    SizeType level ///< [in] ログレベル
  );

  /// @brief ログ出力用ストリームを設定する．
  void
  set_logstream(
    ostream* out ///< [in] 出力ストリーム
  );

  /// @brief 内部の統計情報を出力する．
  void
  dump_stats(
    ostream& s ///< [in] 出力ストリーム
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief lit1 が成り立つか調べる．
  SatBool3
  check_condition(
    SatLiteral lit1
  );

  /// @brief lit1 & lit2 が成り立つか調べる．
  SatBool3
  check_condition(
    SatLiteral lit1,
    SatLiteral lit2
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // SatSolver の統計情報
  struct SatStat
  {

    // 試行回数
    int mTotalCount;

    struct
    {
      // 回数
      int mCount;

      // 計算時間の総和
      double mTotalTime;

      // 計算時間の最大値
      double mMaxTime;

      // restart 回数
      int mRestart;

      // コンフリクト数
      ymuint64 mConflictNum;

      // decision 数
      ymuint64 mDecisionNum;

      // implication 数
      ymuint64 mPropagationNum;

      // 学習説のリテラル数
      ymuint64 mLearntLitNum;

    } mTimeStat[3];

    // コンストラクタ
    SatStat();

    // 結果をセットする．
    // code = SatBool3::True 検証成功
    //      = SatBool3::3False 検証失敗
    //      = SatBool3::X     アボート
    void
    set_result(
      SatBool3 code,
      double t
    );

    // 内容をダンプする．
    void
    dump(
      ostream& s ///< [in] 出力ストリーム
    ) const;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATソルバ
  SatSolver mSolver;

  // check_const の統計情報
  SatStat mCheckConstInfo;

  // check_equiv の統計情報
  SatStat mCheckEquivInfo;

  // recsolver 用のストリーム
  ostream* mOutP;

  // ログレベル
  SizeType mLogLevel;

  // ログ出力用のストリーム
  ostream* mLogStream;

};

END_NAMESPACE_FRAIG

#endif // FRAIGSAT_H
