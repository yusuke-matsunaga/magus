#ifndef LIBYM_CEC_FRAIGMGR_H
#define LIBYM_CEC_FRAIGMGR_H

/// @file ym_cec/FraigMgr.h
/// @brief FraigMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "cec_nsdef.h"
#include "FraigHandle.h"
#include "ym/Expr.h"
#include "ym/SatBool3.h"
#include "ym/SatSolverType.h"


BEGIN_NAMESPACE_YM_CEC

class FraigMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class FraigMgr FraigMgr.h "ym_cec/FraigMgr.h"
/// @brief Functionary Reduced AND-INVERTOR Graph を管理するクラス
//////////////////////////////////////////////////////////////////////
class FraigMgr
{
 public:

  /// @brief コンストラクタ
  /// @brief sig_size シグネチャのサイズ
  /// @param[in] solver_type SAT-solver の種類を表すオブジェクト
  FraigMgr(ymuint sig_size,
	   const SatSolverType& solver_type = SatSolverType());


  /// @brief デストラクタ
  ~FraigMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノード数を得る．
  ymuint
  input_num() const;

  /// @brief 入力ノードを取り出す．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  FraigNode*
  input_node(ymuint pos) const;

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] pos ノード番号 ( 0 <= pos < input_num() )
  /// @note ANDノードの他に入力ノードも含まれる．
  FraigNode*
  node(ymuint pos) const;


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

  /// @brief 2つのノードの AND を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_and(FraigHandle edge1,
	   FraigHandle edge2);

  /// @brief 複数のノードの AND を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  FraigHandle
  make_and(const vector<FraigHandle>& edge_list);

  /// @brief 2つのノードの OR を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_or(FraigHandle edge1,
	  FraigHandle edge2);

  /// @brief 複数のノードの OR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  FraigHandle
  make_or(const vector<FraigHandle>& edge_list);

  /// @brief 2つのノードの XOR を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  FraigHandle
  make_xor(FraigHandle edge1,
	   FraigHandle edge2);

  /// @brief 複数のノードの XOR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  FraigHandle
  make_xor(const vector<FraigHandle>& edge_list);

  /// @brief 論理式に対応するノード(木)をつくる．
  /// @param[in] expr 対象の論理式
  /// @param[in] inputs 入力に対応する AIG ハンドル
  FraigHandle
  make_logic(const Expr& expr,
	     const vector<FraigHandle>& inputs);

  /// @brief コファクターを計算する．
  /// @param[in] edge 対象の AIG ハンドル
  /// @param[in] input_id コファクターをとる入力番号
  /// @param[in] inv 反転フラグ
  FraigHandle
  make_cofactor(FraigHandle edge,
		ymuint input_id,
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
  set_loop_limit(ymuint loop_limit);

  /// @brief 内部の統計情報を出力する．
  void
  dump_stats(ostream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 複数のノードの AND を取る．
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
	    ymuint start_pos,
	    ymuint end_pos,
	    bool iinv);

  /// @brief 複数のノードの XOR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  /// @param[in] start_pos 開始位置
  /// @param[in] end_pos 終了位置
  ///
  /// edge_list[start_pos] から edge_list[end_pos - 1] までの
  /// ハンドルの XOR を取る．
  /// なので常に end_pos > start_pos が成り立つと仮定する．
  FraigHandle
  _make_xor(const vector<FraigHandle>& edge_list,
	    ymuint start_pos,
	    ymuint end_pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行う実装クラス
  std::auto_ptr<FraigMgrImpl> mImpl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 複数のノードの AND を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
FraigHandle
FraigMgr::make_and(const vector<FraigHandle>& edge_list)
{
  ymuint n = edge_list.size();
  ASSERT_COND( n > 0 );
  return _make_and(edge_list, 0, n, false);
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
  ymuint n = edge_list.size();
  ASSERT_COND( n > 0 );
  return ~_make_and(edge_list, 0, n, true);
}

// @brief 2つのノードの XOR を取る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
FraigHandle
FraigMgr::make_xor(FraigHandle edge1,
		   FraigHandle edge2)
{
  FraigHandle tmp1 = make_and(edge1, ~edge2);
  FraigHandle tmp2 = make_and(~edge1, edge2);
  return make_or(tmp1, tmp2);
}

// @brief 複数のノードの XOR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
FraigHandle
FraigMgr::make_xor(const vector<FraigHandle>& edge_list)
{
  ymuint n = edge_list.size();
  ASSERT_COND( n > 0 );
  return _make_xor(edge_list, 0, n);
}

END_NAMESPACE_YM_CEC

#endif // LIBYM_CEC_FRAIGMGR_H
