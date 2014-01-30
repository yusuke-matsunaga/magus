#ifndef GBMSATENGINE_H
#define GBMSATENGINE_H

/// @file GbmSatEngine.h
/// @brief GbmSatEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmEngine.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmSatEngine GbmSatEngine.h "GbmSatEngine.h"
/// @brief SAT ベースの GbmEngine
//////////////////////////////////////////////////////////////////////
class GbmSatEngine :
  public GbmEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] solver SATソルバ
  /// @param[in] node_num ノード数
  /// @param[in] conf_num 設定変数の数
  GbmSatEngine(SatSolver& solver,
	       ymuint node_num,
	       ymuint conf_num);

  /// @brief デストラクタ
  ~GbmSatEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 設定変数を初期化する．
  void
  init_conf_vars();

  /// @brief SAT モデルから設定変数の割り当てを取り出す．
  /// @param[in] model SAT モデル
  /// @param[out] conf_bits 設定変数の割り当て
  void
  get_conf_bits(const vector<Bool3>& model,
		vector<bool>& conf_bits) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに対応するリテラルを登録する．
  /// @param[in] id ノード番号
  /// @param[in] lit リテラル
  void
  set_node_var(ymuint id,
	       GbmLit lit);

  /// @brief 内部ノードに変数番号を割り当て，CNF式を作る．
  /// @param[in] network 対象の LUT ネットワーク
  /// @param[in] oval 出力値
  /// @return 割り当てが矛盾を起こしたら false を返す．
  bool
  make_nodes_cnf(const RcfNetwork& network,
		 ymuint oval);

  /// @brief SAT用の新しい変数を作る．
  VarId
  new_var();

  /// @brief 節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2);

  /// @brief 節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2,
	     Literal lit3);

  /// @brief 節を追加する．
  void
  add_clause(Literal lit1,
	     Literal lit2,
	     Literal lit3,
	     Literal lit4);

  /// @brief 節を追加する．
  void
  add_clause(const vector<Literal>& lits);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの入出力の関係を表す CNF 式を作る．
  /// @param[in] node 対象のノード
  /// @return 割り当てが矛盾を起こしたら false を返す．
  bool
  make_node_cnf(const RcfNode* node);

  /// @brief AND ゲートを表す節を追加する．
  /// @param[in] input_vars 入力変数のリスト
  /// @param[in] output_var 出力変数
  /// @return 割り当てが矛盾を起こしたら false を返す．
  bool
  make_AND(const vector<GbmLit>& input_vars,
	   GbmLit output_var);

  /// @brief LUT を表す節を追加する．
  /// @param[in] input_vars 入力変数のリスト
  /// @param[in] lut_vars LUT変数のリスト
  /// @param[in] output_var 出力変数
  /// @note lut_vars のサイズは input_vars のサイズの指数乗
  /// @return 割り当てが矛盾を起こしたら false を返す．
  bool
  make_LUT(const vector<GbmLit>& input_vars,
	   const vector<GbmLit>& lut_vars,
	   GbmLit output_var);

  /// @brief MUX を表す節を追加する．
  /// @param[in] d_vars データ入力変数のリスト
  /// @param[in] s_vars 選択信号変数のリスト
  /// @param[in] output_var 出力変数
  /// @note d_vars のサイズは s_vars のサイズの指数乗
  /// @return 割り当てが矛盾を起こしたら false を返す．
  bool
  make_MUX(const vector<GbmLit>& d_vars,
	   const vector<GbmLit>& s_vars,
	   GbmLit output_var);

  /// @brief RcfNodeHandle から GbmLit を作る．
  /// @param[in] handle ハンドル
  GbmLit
  handle_to_lit(RcfNodeHandle handle);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATソルバ
  SatSolver& mSolver;

  // ノードIDをキーにしてリテラルを格納する配列
  vector<GbmLit> mNodeVarArray;

  // 設定変数番号をキーにしてSATソルバ上の変数番号を格納する配列
  vector<VarId> mConfVarArray;

  // debug フラグ
  bool mDebug;

};

END_NAMESPACE_YM

#endif // GBMSATENGINE_H
