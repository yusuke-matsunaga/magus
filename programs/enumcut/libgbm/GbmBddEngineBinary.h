#ifndef GBMBDDENGINEBINARY_H
#define GBMBDDENGINEBINARY_H

/// @file GbmBddEngineBinary.h
/// @brief GbmBddEngineBinary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "logic/Bdd.h"
#include "logic/BddMgr.h"
#include "logic/Bool3.h"
#include "RcfNetwork.h"
#include "RcfNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmBddEngineBinary GbmBddEngineBinary.h "GbmBddEngineBinary.h"
/// @brief BDD ベースの GbmEngine
//////////////////////////////////////////////////////////////////////
class GbmBddEngineBinary
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr BddMgr
  GbmBddEngineBinary(BddMgr& mgr);

  /// @brief デストラクタ
  ~GbmBddEngineBinary();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief debug フラグを立てる
  void
  debug_on();

  /// @brief debug フラグを降ろす
  void
  debug_off();

  /// @brief debug フラグの値を得る．
  bool
  debug() const;

  /// @brief 対称性を考慮して初期解を作る．
  /// @param[in] network 対象の LUT ネットワーク
  /// @param[in] rep 関数の対称変数の代表番号を収める配列
  void
  init_vars(const RcfNetwork& network,
	    const vector<ymuint>& rep);

  /// @brief 入力値を割り当てて解の候補を求める．
  /// @param[in] network 対象の LUT ネットワーク
  /// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
  /// @param[in] oval 出力の値
  /// @param[out] model モデル
  /// @return 結果が空でなければ true を返し，model にその1つを収める．
  Bool3
  make_bdd(const RcfNetwork& network,
	   ymuint bitpat,
	   bool oval);

  /// @brief 結果からモデルを一つ取り出す．
  void
  get_model(vector<Bool3>& model);

  /// @brief SAT モデルから設定変数の割り当てを取り出す．
  /// @param[in] model SAT モデル
  /// @param[out] conf_bits 設定変数の割り当て
  void
  get_conf_bits(const vector<Bool3>& model,
		vector<bool>& conf_bits) const;

  /// @brief SAT モデルから入力順を取り出す．
  /// @param[in] model SAT モデル
  /// @param[out] iorder 入力順
  ///             iorder[pos] に network の pos 番めの入力に対応した
  ///             関数の入力番号が入る．
  void
  get_iorder(const vector<Bool3>& model,
	     vector<ymuint>& iorder) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数順を表すBDDを作る．
  /// @param[in] level レベル
  /// @param[in] var_list 変数の集合を表すビットベクタ
  /// @param[in] network 対象の LUT ネットワーク
  ///
  /// var_list に含まれる変数の順列をすべて表す
  /// BDD を返す．
  Bdd
  make_iorder_bdd(ymuint level,
		  ymuint var_list,
		  const RcfNetwork& network);

  /// @brief RcfNode に対応する関数を計算する．
  /// @param[in] node 対象のノード
  /// @note 結果は mNodeBddArray に格納される．
  void
  make_node_func(const RcfNode* node);

  /// @brief LUT ノードの出力の論理関数を計算する．
  /// @param[in] inputs ファンインの論理関数
  /// @param[in] lut_vars LUT のコンフィグレーションメモリ
  Bdd
  make_LUT(const vector<Bdd>& inputs,
	   const vector<Bdd>& lut_vars);

  /// @brief セレクタの出力の論理関数を計算する．
  /// @param[in] inputs ファンインの論理関数
  /// @param[in] s_vars 選択変数
  Bdd
  make_MUX(const vector<Bdd>& inputs,
	   const vector<Bdd>& s_vars);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgr
  BddMgr& mMgr;

  // 設定変数番号をキーにして BDD上の変数番号を格納する配列
  vector<VarId> mConfVarArray;

  // 入力数
  ymuint32 mInputNum;


  // 入力順を表す変数の数
  ymuint32 mIorderBitWidth;

  // 入力順を表す変数の配列
  // i * mIorderBitWidth から mIorderBitWidth 分の変数が LUT network の i 番めの入力と
  // 接続している関数の入力番号を表す．
  vector<VarId> mIorderVarArray;

  // 各 RcfNode の関数を格納する配列
  vector<Bdd> mNodeBddArray;

  // var_list をキーにして順列の集合を表すBDDを格納する配列
  vector<Bdd> mIorderBddArray;

  // 途中結果を表す BDD
  Bdd mSolution;

  // デバッグフラグ
  bool mDebug;
};

END_NAMESPACE_YM


#endif // GBMBDDENGINEBINARY_H
