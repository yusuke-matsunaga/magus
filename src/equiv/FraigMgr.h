#ifndef FRAIGMGR_H
#define FRAIGMGR_H

/// @file FraigMgr.h
/// @brief FraigMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "equiv.h"
#include "FraigHandle.h"
#include "StructHash.h"
#include "PatHash.h"
#include "ym/Expr.h"
#include "ym/SimpleAlloc.h"
#include "ym/RandGen.h"
#include "ym/SatBool3.h"
#include "ym/SatSolverType.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_EQUIV

//////////////////////////////////////////////////////////////////////
/// @class FraigMgr FraigMgr.h "FraigMgr.h"
/// @brief Functionary Reduced AND-INVERTOR Graph を管理するクラス
//////////////////////////////////////////////////////////////////////
class FraigMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] sig_size シグネチャのサイズ
  /// @param[in] solver_type SAT-solver の種類を表すオブジェクト
  FraigMgr(int sig_size,
	   const SatSolverType& solver_type = SatSolverType());


  /// @brief デストラクタ
  ~FraigMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノード数を得る．
  int
  input_num() const;

  /// @brief 入力ノードを取り出す．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  FraigNode*
  input_node(int pos) const;

  /// @brief ノード数を得る．
  int
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] pos ノード番号 ( 0 <= pos < input_num() )
  /// @note ANDノードの他に入力ノードも含まれる．
  FraigNode*
  node(int pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 構造(FraigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0関数をつくる．
  FraigHandle
  make_zero();

  /// @brief 定数1関数をつくる．
  FraigHandle
  make_one();

  /// @brief 外部入力を作る．
  FraigHandle
  make_input();

  /// @brief バッファを作る．
  FraigHandle
  make_buff(FraigHandle edge1);

  /// @brief NOTを作る．
  FraigHandle
  make_not(FraigHandle edge1);

  /// @brief 2つのノードの AND を作る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_and(FraigHandle edge1,
	   FraigHandle edge2);

  /// @brief 複数のノードの AND を作る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  FraigHandle
  make_and(const vector<FraigHandle>& edge_list);

  /// @brief 2つのノードの NAND を作る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_nand(FraigHandle edge1,
	    FraigHandle edge2);

  /// @brief 複数のノードの NAND を作る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  FraigHandle
  make_nand(const vector<FraigHandle>& edge_list);

  /// @brief 2つのノードの OR を作る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_or(FraigHandle edge1,
	  FraigHandle edge2);

  /// @brief 複数のノードの OR を作る．．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  FraigHandle
  make_or(const vector<FraigHandle>& edge_list);

  /// @brief 2つのノードの NOR を作る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_nor(FraigHandle edge1,
	   FraigHandle edge2);

  /// @brief 複数のノードの NOR を作る．．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  FraigHandle
  make_nor(const vector<FraigHandle>& edge_list);

  /// @brief 2つのノードの XOR を作る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_xor(FraigHandle edge1,
	   FraigHandle edge2);

  /// @brief 複数のノードの XOR を作る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  FraigHandle
  make_xor(const vector<FraigHandle>& edge_list);

  /// @brief 2つのノードの XNOR を作る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_xnor(FraigHandle edge1,
	    FraigHandle edge2);

  /// @brief 複数のノードの XNOR を作る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  FraigHandle
  make_xnor(const vector<FraigHandle>& edge_list);

  /// @brief 論理式に対応するノード(木)を作る．
  /// @param[in] expr 対象の論理式
  /// @param[in] inputs 入力に対応する AIG ハンドル
  FraigHandle
  make_expr(const Expr& expr,
	    const vector<FraigHandle>& inputs);

  /// @brief コファクターを計算する．
  /// @param[in] edge 対象の AIG ハンドル
  /// @param[in] input_id コファクターをとる入力番号
  /// @param[in] inv 反転フラグ
  FraigHandle
  make_cofactor(FraigHandle edge,
		int input_id,
		bool inv);


public:
  //////////////////////////////////////////////////////////////////////
  // 検証用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのハンドルが等価かどうか調べる．
  SatBool3
  check_equiv(FraigHandle aig1,
	      FraigHandle aig2);

  /// @brief ログレベルを設定する．
  void
  set_loglevel(int level);

  /// @brief ログ出力用ストリームを設定する．
  void
  set_logstream(ostream* out);

  /// @brief ランダムシミュレーション制御用のパラメータを設定する．
  /// @param[in] loop_limit 変化のない状態がこの回数連続したら止める．
  void
  set_loop_limit(int loop_limit);

  /// @brief 内部の統計情報を出力する．
  void
  dump_stats(ostream& s);


public:
  //////////////////////////////////////////////////////////////////////
  // PatHash で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタンハッシュで同値と判定されたノードを比較する．
  bool
  compare_node(FraigNode* node1,
	       FraigNode* node2,
	       bool inv,
	       bool& retry);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief make_and() の下請け関数
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  /// @param[in] start_pos 開始位置
  /// @param[in] end_pos 終了位置
  /// @param[in] iinv 入力の反転フラグ
  ///
  /// edge_list[start_pos] から edge_list[end_pos - 1] までの
  /// ハンドルの AND を取る．
  /// なので常に end_pos > start_pos が成り立つと仮定する．
  FraigHandle
  _make_and(const vector<FraigHandle>& edge_list,
	    int start_pos,
	    int end_pos,
	    bool iinv);

  /// @brief make_xor() の下請け関数
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  /// @param[in] start_pos 開始位置
  /// @param[in] end_pos 終了位置
  ///
  /// edge_list[start_pos] から edge_list[end_pos - 1] までの
  /// ハンドルの XOR を取る．
  /// なので常に end_pos > start_pos が成り立つと仮定する．
  FraigHandle
  _make_xor(const vector<FraigHandle>& edge_list,
	    int start_pos,
	    int end_pos);

  /// @brief ノードの入出力の関係を表す CNF 式を作る．
  /// @param[in] node 対象のノード
  void
  make_cnf(FraigNode* node);

  /// @brief 縮退検査を行う．
  /// @param[in] node 対象のノード
  /// @param[out] ans 答
  /// @retval SatBool3::True 定数に縮退していた．
  /// @retval SatBool3::False 定数ではなかった．
  /// @retval SatBool3::X 不明(アボート)
  SatBool3
  verify_const(FraigNode* node,
	       FraigHandle& ans);

  /// @brief パタンハッシュを調べて論理的に等価なノードを探す．
  /// @param[in] node 対象のノード
  /// @return 等価なハンドル
  ///
  /// 等価なノードが見つからなかった場合はパタンハッシュに登録する．
  FraigHandle
  check_pat(FraigNode* node);

  /// @brief ノードのシミュレーションパタン用配列を確保する．
  /// @param[in] node 対象のノード
  void
  init_pat(FraigNode* node);

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

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

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
  RandGen mRandGen;

  // SATソルバ
  SatSolver mSolver;

  // SAT 用の割り当て格納配列
  vector<SatBool3> mModel;

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
FraigMgr::input_num() const
{
  return mInputNodes.size();
}

// @brief 入力ノードを取り出す．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
inline
FraigNode*
FraigMgr::input_node(int pos) const
{
  ASSERT_COND( pos >= 0 && pos < input_num() );

  return mInputNodes[pos];
}

// @brief ノード数を得る．
inline
int
FraigMgr::node_num() const
{
  return mAllNodes.size();
}

// @brief ノードを取り出す．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
// @note ANDノードの他に入力ノードも含まれる．
inline
FraigNode*
FraigMgr::node(int pos) const
{
  ASSERT_COND( pos >= 0 && pos < node_num() );

  return mAllNodes[pos];
}

// @brief 定数0関数をつくる．
inline
FraigHandle
FraigMgr::make_zero()
{
  return FraigHandle(nullptr, false);
}

// @brief 定数1関数をつくる．
inline
FraigHandle
FraigMgr::make_one()
{
  return FraigHandle(nullptr, true);
}

// @brief バッファを作る．
inline
FraigHandle
FraigMgr::make_buff(FraigHandle edge1)
{
  return edge1;
}

// @brief NOTを作る．
inline
FraigHandle
FraigMgr::make_not(FraigHandle edge1)
{
  return ~edge1;
}

// @brief 複数のノードの AND を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
FraigHandle
FraigMgr::make_and(const vector<FraigHandle>& edge_list)
{
  int n = edge_list.size();
  ASSERT_COND( n > 0 );
  return _make_and(edge_list, 0, n, false);
}

// @brief 2つのノードの NAND を作る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
FraigHandle
FraigMgr::make_nand(FraigHandle edge1,
		    FraigHandle edge2)
{
  return ~make_and(edge1, edge2);
}

// @brief 複数のノードの NAND を作る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
FraigHandle
FraigMgr::make_nand(const vector<FraigHandle>& edge_list)
{
  int n = edge_list.size();
  ASSERT_COND( n > 0 );
  return ~_make_and(edge_list, 0, n, false);
}

// @brief 2つのノードの OR を取る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
FraigHandle
FraigMgr::make_or(FraigHandle edge1,
		  FraigHandle edge2)
{
  return ~make_and(~edge1, ~edge2);
}

// @brief 複数のノードの OR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
FraigHandle
FraigMgr::make_or(const vector<FraigHandle>& edge_list)
{
  int n = edge_list.size();
  ASSERT_COND( n > 0 );
  return ~_make_and(edge_list, 0, n, true);
}

// @brief 2つのノードの NOR を作る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
FraigHandle
FraigMgr::make_nor(FraigHandle edge1,
		   FraigHandle edge2)
{
  return make_and(~edge1, ~edge2);
}

// @brief 複数のノードの NOR を作る．．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
FraigHandle
FraigMgr::make_nor(const vector<FraigHandle>& edge_list)
{
  int n = edge_list.size();
  ASSERT_COND( n > 0 );
  return _make_and(edge_list, 0, n, true);
}

// @brief 2つのノードの XOR を取る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
FraigHandle
FraigMgr::make_xor(FraigHandle edge1,
		   FraigHandle edge2)
{
  FraigHandle tmp1 = make_and( edge1, ~edge2);
  FraigHandle tmp2 = make_and(~edge1,  edge2);
  return make_or(tmp1, tmp2);
}

// @brief 複数のノードの XOR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
FraigHandle
FraigMgr::make_xor(const vector<FraigHandle>& edge_list)
{
  int n = edge_list.size();
  ASSERT_COND( n > 0 );
  return _make_xor(edge_list, 0, n);
}

// @brief 2つのノードの XNOR を作る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
FraigHandle
FraigMgr::make_xnor(FraigHandle edge1,
		    FraigHandle edge2)
{
  FraigHandle tmp1 = make_and( edge1, ~edge2);
  FraigHandle tmp2 = make_and(~edge1,  edge2);
  return make_nor(tmp1, tmp2);
}

// @brief 複数のノードの XNOR を作る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
FraigHandle
FraigMgr::make_xnor(const vector<FraigHandle>& edge_list)
{
  int n = edge_list.size();
  ASSERT_COND( n > 0 );
  return ~_make_xor(edge_list, 0, n);
}

END_NAMESPACE_EQUIV

#endif // FRAIGMGR_H
