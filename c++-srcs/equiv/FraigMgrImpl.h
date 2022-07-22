#ifndef FRAIGMGRIMPL_H
#define FRAIGMGRIMPL_H

/// @file FraigMgrImpl.h
/// @brief FraigMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "fraig.h"
#include "FraigHandle.h"
#include "StructHash.h"
#include "PatHash.h"
#include "ym/Expr.h"
#include "ym/SatBool3.h"
#include "ym/SatSolverType.h"
#include "ym/SatSolver.h"
#include "ym/SatModel.h"
#include <random>


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class FraigMgrImpl FraigMgrImpl.h "FraigMgrImpl.h"
/// @brief FraigMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class FraigMgrImpl
{
public:

  /// @brief コンストラクタ
  FraigMgrImpl(
    SizeType sig_size,               ///< [in] シグネチャのサイズ
    const SatSolverType& solver_type ///< [in] SAT-solver の種類を表すオブジェクト
    = SatSolverType()
  );

  /// @brief デストラクタ
  ~FraigMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノード数を得る．
  SizeType
  input_num() const;

  /// @brief 入力ノードを取り出す．
  FraigNode*
  input_node(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < input_num() )
  ) const;

  /// @brief ノード数を得る．
  SizeType
  node_num() const;

  /// @brief ノードを取り出す．
  ///
  /// ANDノードの他に入力ノードも含まれる．
  FraigNode*
  node(
    SizeType pos ///< [in] ノード番号 ( 0 <= pos < node_num() )
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 構造(FraigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力を作る．
  FraigHandle
  make_input();

  /// @brief 2つのノードの AND を作る．
  FraigHandle
  make_and(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 検証用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのハンドルが等価かどうか調べる．
  SatBool3
  check_equiv(
    FraigHandle aig1, ///< [in] 入力1のハンドル
    FraigHandle aig2  ///< [in] 入力2のハンドル
  );

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

  /// @brief ランダムシミュレーション制御用のパラメータを設定する．
  void
  set_loop_limit(
    SizeType loop_limit ///< [in] 変化のない状態がこの回数連続したら止める．
  );

  /// @brief 内部の統計情報を出力する．
  void
  dump_stats(
    ostream& s ///< [in] 出力ストリーム
  );


public:
  //////////////////////////////////////////////////////////////////////
  // PatHash で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタンハッシュで同値と判定されたノードを比較する．
  bool
  compare_node(
    FraigNode* node1,
    FraigNode* node2,
    bool inv,
    bool& retry
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの入出力の関係を表す CNF 式を作る．
  void
  make_cnf(
    FraigNode* node ///< [in] 対象のノード
  );

  /// @brief 縮退検査を行う．
  /// @retval SatBool3::True 定数に縮退していた．
  /// @retval SatBool3::False 定数ではなかった．
  /// @retval SatBool3::X 不明(アボート)
  SatBool3
  verify_const(
    FraigNode* node, ///< [in] 対象のノード
    FraigHandle& ans ///< [out] 答
  );

  /// @brief パタンハッシュを調べて論理的に等価なノードを探す．
  /// @return 等価なハンドル
  ///
  /// 等価なノードが見つからなかった場合はパタンハッシュに登録する．
  FraigHandle
  check_pat(
    FraigNode* node ///< [in] 対象のノード
  );

  /// @brief ノードのシミュレーションパタン用配列を確保する．
  void
  init_pat(
    FraigNode* node ///< [in] 対象のノード
  );

  /// @brief 全ノードのシミュレーションパタン用配列を拡大する．
  void
  resize_pat(int size);

  /// @breif 直前の SAT の反例を加えて再ハッシュする．
  void
  add_pat(FraigNode* node);

  /// @brief ノードが定数と等価かどうか調べる．
  /// @param[in] node 対象のノード
  /// @param[in] inv false で 0, true で 1 を表す．
  SatBool3
  check_const(FraigNode* node,
	      bool inv);

  /// @brief 2つのノードが等価かどうか調べる．
  /// @param[in] node1, node2 対象のノード
  /// @param[in] inv false で同相，true で逆相を表す．
  SatBool3
  check_equiv(FraigNode* node1,
	      FraigNode* node2,
	      bool inv);

  /// @brief lit1 が成り立つか調べる．
  SatBool3
  check_condition(SatLiteral lit1);

  /// @brief lit1 & lit2 が成り立つか調べる．
  SatBool3
  check_condition(SatLiteral lit1,
		  SatLiteral lit2);

  /// @brief FraigHandle に対応するリテラルを返す．
  /// @note 定数の場合の返り値は未定
  SatLiteral
  fraig2literal(FraigHandle aig);

  /// @brief 等価候補グループの情報を出力する．
  void
  dump_eqgroup(ostream& s) const;

  /// @brief 新しいノードを生成する．
  FraigNode*
  new_node();

  /// @brief シミュレーションパタンが等しいか調べる．
  bool
  compare_pat(FraigNode* node1,
	      FraigNode* node2,
	      bool inv);


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
    // code = kB3True 検証成功
    //      = kB3False 検証失敗
    //      = kB3X     アボート
    void
    set_result(SatBool3 code,
	       double t);

    // 内容をダンプする．
    void
    dump(ostream& s) const;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号をキーにしたノードの配列
  vector<FraigNode*> mAllNodes;

  // 入力ノードの配列
  vector<FraigNode*> mInputNodes;

  // 構造ハッシュ
  StructHash mHashTable1;

  // 各ノードのシミュレーションパタンのサイズ
  int mPatSize;

  // 初期パタン数
  int mPatInit;

  // 使用しているパタン数
  int mPatUsed;

  // パタンハッシュ
  PatHash mHashTable2;

  // 乱数発生器
  std::mt19937 mRandGen;

  // SATソルバ
  SatSolver mSolver;

  // SAT 用の割り当て格納配列
  SatModel mModel;

  // sat_sweep 中のシミュレーション回数
  int mSimCount;

  // シミュレーションに要した時間
  double mSimTime;

  // check_const の統計情報
  SatStat mCheckConstInfo;

  // check_equiv の統計情報
  SatStat mCheckEquivInfo;

  // recsolver 用のストリーム
  ostream* mOutP;

  // ログレベル
  int mLogLevel;

  // ログ出力用のストリーム
  ostream* mLogStream;

  // シミュレーションのループ回数
  int mLoopLimit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力ノード数を得る．
inline
int
FraigMgrImpl::input_num() const
{
  return mInputNodes.size();
}

// @brief 入力ノードを取り出す．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
inline
FraigNode*
FraigMgrImpl::input_node(int pos) const
{
  ASSERT_COND( pos >= 0 && pos < input_num() );

  return mInputNodes[pos];
}

// @brief ノード数を得る．
inline
int
FraigMgrImpl::node_num() const
{
  return mAllNodes.size();
}

// @brief ノードを取り出す．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
// @note ANDノードの他に入力ノードも含まれる．
inline
FraigNode*
FraigMgrImpl::node(int pos) const
{
  ASSERT_COND( pos >= 0 && pos < node_num() );

  return mAllNodes[pos];
}

END_NAMESPACE_FRAIG

#endif // FRAIGMGRIMPL_H
