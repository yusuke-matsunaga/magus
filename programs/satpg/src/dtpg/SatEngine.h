#ifndef SATENGINE_H
#define SATENGINE_H

/// @file SatEngine.h
/// @brief SatEngine のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "TpgNode.h"
#include "DtpgStats.h"
#include "LitMap.h"
#include "logic/Literal.h"
#include "logic/Bool3.h"
#include "logic/sat_nsdef.h"
#include "logic/SatStats.h"
#include "utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngine SatEngine.h "SatEngine.h"
/// @brief SAT ベースのATPGエンジン
//////////////////////////////////////////////////////////////////////
class SatEngine
{
public:

  /// @brief コンストラクタ
  SatEngine();

  /// @brief デストラクタ
  virtual
  ~SatEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL);

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  run(const vector<TpgFault*>& flist,
      const vector<TpgNode*>& fnode_list,
      ymuint max_id,
      BackTracer& bt,
      DetectOp& dop,
      UntestOp& uop);

  /// @brief 統計情報をクリアする．
  void
  clear_stats();

  /// @brief 統計情報を得る．
  /// @param[in] stats 結果を格納する構造体
  void
  get_stats(DtpgStats& stats) const;

  /// @breif 時間計測を制御する．
  void
  timer_enable(bool enable);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief SATソルバのタイプを得る．
  string
  sat_type() const;

  /// @brief SATソルバのオプションを得る．
  string
  sat_option() const;

  /// @brief SATソルバのログ出力を得る．
  ostream*
  sat_outp() const;

  /// @brief CNF 作成を開始する．
  void
  cnf_begin();

  /// @brief CNF 作成を終了する．
  void
  cnf_end();

  /// @brief 故障位置を与えてその TFO の TFI リストを作る．
  /// @param[in] solver SAT ソルバ
  /// @param[in] fnode_list 故障位置のノードのリスト
  /// @param[in] max_id ノード番号の最大値
  ///
  /// 結果は mTfoList に格納される．
  /// 故障位置の TFO が mTfoList の [0: mTfoEnd - 1] に格納される．
  void
  mark_region(SatSolver& solver,
	      const vector<TpgNode*>& fnode_list,
	      ymuint max_id);

  /// @brief TFO ノードの数を得る．
  ymuint
  tfo_size() const;

  /// @brief TFI ノードの数を得る．
  ymuint
  tfi_size() const;

  /// @brief TFO ノードと TFI ノードの総数を得る．
  ymuint
  tfo_tfi_size() const;

  /// @brief TFO/TFI ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < tfo_tfi_size() )
  ///
  /// pos が tfo_size() 未満のときは TFO ノード
  /// それ以上は TFI ノードとなっている．
  TpgNode*
  tfo_tfi_node(ymuint pos) const;

  /// @brief 出力のノードのリストを返す．
  const vector<TpgNode*>&
  output_list() const;

  /// @brief 節の作成用の作業領域の使用を開始する．
  /// @param[in] exp_size 予想されるサイズ
  void
  tmp_lits_begin(ymuint exp_size = 0);

  /// @brief 作業領域にリテラルを追加する．
  void
  tmp_lits_add(Literal lit);

  /// @brief 作業領域の冊を SAT ソルバに加える．
  void
  tmp_lits_end(SatSolver& solver);

  /// @brief ノードの入出力の関係を表す CNF を作る．
  /// @param[in] solver SATソルバ
  /// @param[in] node 対象のノード
  /// @param[in] litmap 入出力のリテラルを保持するクラス
  static
  void
  make_node_cnf(SatSolver& solver,
		TpgNode* node,
		const LitMap& litmap);

  /// @brief 一つの SAT問題を解く．
  Bool3
  solve(SatSolver& solver,
	TpgFault* f,
	BackTracer& bt,
	DetectOp& dop,
	UntestOp& uop);

  /// @brief ノードの変数割り当てフラグを消す．
  void
  clear_node_mark();

  /// @brief tmp マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tmp_mark(TpgNode* node);

  /// @brief tmp マークを消す．
  /// @param[in] node 対象のノード
  void
  clear_tmp_mark(TpgNode* node);

  /// @brief tmp マークを読む．
  /// @param[in] node 対象のノード
  bool
  tmp_mark(TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief tfo マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tfo_mark(TpgNode* node);

  /// @brief tfo マークを読む．
  /// @param[in] node 対象のノード
  bool
  tfo_mark(TpgNode* node);

  /// @brief tfi マークをつける．
  /// @param[in] node 対象のノード
  void
  set_tfi_mark(TpgNode* node);

  /// @brief tfi マークを読む．
  /// @param[in] node 対象のノード
  bool
  tfi_mark(TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT solver のタイプ
  string mType;

  // SAT solver のオプション
  string mOption;

  // SAT solver の記録用ストリーム
  ostream* mOutP;

  // SAT の結果を格納する配列
  vector<Bool3> mModel;

  // ノードごとのいくつかのフラグをまとめた配列
  vector<ymuint8> mMarkArray;

  // 故障の TFO のノードリスト
  vector<TpgNode*> mTfoList;

  // TFO ノードの最後の位置
  ymuint32 mTfoEnd;

  // 現在の故障に関係のありそうな外部入力のリスト
  vector<TpgNode*> mInputList;

  // 現在の故障に関係ありそうな外部出力のリスト
  vector<TpgNode*> mOutputList;

  // 作業用のリテラルのリスト
  vector<Literal> mTmpLits;

  // 時間計測を行なうかどうかの制御フラグ
  bool mTimerEnable;

  // 時間計測用のタイマー
  StopWatch mTimer;

  // DTPG の統計情報
  DtpgStats mStats;

  // SATソルバの前回の統計情報
  SatStats mPrevStats;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief SATソルバのタイプを得る．
inline
string
SatEngine::sat_type() const
{
  return mType;
}

// @brief SATソルバのオプションを得る．
inline
string
SatEngine::sat_option() const
{
  return mOption;
}

// @brief SATソルバのログ出力を得る．
inline
ostream*
SatEngine::sat_outp() const
{
  return mOutP;
}

// @brief TFO ノードの数を得る．
inline
ymuint
SatEngine::tfo_size() const
{
  return mTfoEnd;
}

// @brief TFI ノードの数を得る．
inline
ymuint
SatEngine::tfi_size() const
{
  return mTfoList.size() - mTfoEnd;
}

// @brief TFO ノードと TFI ノードの総数を得る．
inline
ymuint
SatEngine::tfo_tfi_size() const
{
  return mTfoList.size();
}

// @brief TFO/TFI ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < tfo_tfi_size() )
//
// pos が tfo_size() 未満のときは TFO ノード
// それ以上は TFI ノードとなっている．
inline
TpgNode*
SatEngine::tfo_tfi_node(ymuint pos) const
{
  return mTfoList[pos];
}

// @brief 出力のノードのリストを返す．
inline
const vector<TpgNode*>&
SatEngine::output_list() const
{
  return mOutputList;
}

// tfo マークをつける．
inline
void
SatEngine::set_tfo_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 1U;
  mTfoList.push_back(node);
  if ( node->is_output() ) {
    mOutputList.push_back(node);
  }
}

// @brief tfo マークを読む．
inline
bool
SatEngine::tfo_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 0) & 1U);
}

// tfi マークをつける．
inline
void
SatEngine::set_tfi_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 2U;
  mTfoList.push_back(node);
  if ( node->is_input() ) {
    mInputList.push_back(node);
  }
}

// @brief tfi マークを読む．
inline
bool
SatEngine::tfi_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 1) & 1U);
}

// @brief tmp マークをつける．
inline
void
SatEngine::set_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] |= 4U;
}

// @brief tmp マークを消す．
inline
void
SatEngine::clear_tmp_mark(TpgNode* node)
{
  mMarkArray[node->id()] &= ~4U;
}

// @brief tmp マークを読む．
inline
bool
SatEngine::tmp_mark(TpgNode* node)
{
  return static_cast<bool>((mMarkArray[node->id()] >> 2) & 1U);
}

END_NAMESPACE_YM_SATPG

#endif // SATENGINEIMPL_H
