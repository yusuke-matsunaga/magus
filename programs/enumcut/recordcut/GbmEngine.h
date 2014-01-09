#ifndef GBMENGINE_H
#define GBMENGINE_H

/// @file GbmEngine.h
/// @brief GbmEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "GbmLit.h"
#include "RcfNode.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmEngine GbmEngine.h "GbmEngine.h"
/// @brief GBM の基本処理を行なうクラス
//////////////////////////////////////////////////////////////////////
class GbmEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] solver SATソルバ
  /// @param[in] node_num ノード数
  /// @param[in] conf_num 設定変数の数
  GbmEngine(SatSolver& solver,
	    ymuint node_num,
	    ymuint conf_num);

  /// @brief デストラクタ
  ~GbmEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに対応するリテラルを登録する．
  /// @param[in] id ノード番号
  /// @param[in] lit リテラル
  void
  set_node_var(ymuint id,
	       GbmLit lit);

  /// @brief 設定変数に対応するリテラルを登録する．
  /// @param[in] id 変数番号
  /// @param[in] lit リテラル
  void
  set_conf_var(ymuint id,
	       GbmLit lit);

  /// @brief 外部入力の順列入れ替えを2値符号化する場合の CNF 式を作る．
  /// @param[in] input_list 入力ノードのリスト
  /// @param[in] iorder_vid_array 入力順を表す変数の配列
  /// @param[in] bw 1つの入力の符号化に用いるビット長
  /// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
  void
  make_inputs_cnf_binary(const vector<const RcfNode*>& input_list,
			 const vector<VarId> iorder_vid_array,
			 ymuint bw,
			 ymuint bit_pat);

  /// @brief 外部入力の順列入れ替えをone-hot符号化する場合の CNF 式を作る．
  /// @param[in] input_list 入力ノードのリスト
  /// @param[in] pred_list 各入力に対する先行者の情報を入れた配列
  /// @param[in] iorder_vid_array 入力順を表す変数の配列
  /// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
  /// @note pred_list[] が -1 の時は先行者なし
  void
  make_inputs_cnf_onehot(const vector<const RcfNode*>& input_list,
			const vector<int>& pred_list,
			const vector<VarId> iorder_vid_array,
			ymuint bit_pat);

  /// @brief 内部ノードに変数番号を割り当て，CNF式を作る．
  /// @param[in] node_list ノードリスト(入力と出力は含まない)
  /// @param[in] oid 出力のノード番号
  /// @param[in] oval 出力値
  /// @return 割り当てが矛盾を起こしたら false を返す．
  bool
  make_nodes_cnf(const vector<const RcfNode*>& node_list,
		 ymuint oid,
		 ymuint oval);


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

  // 設定変数番号をキーにしてリテラルを格納する配列
  vector<GbmLit> mConfVarArray;

};

END_NAMESPACE_YM

#endif // GBMENGINE_H
