#ifndef GBMBDDENGINEENUM_H
#define GBMBDDENGINEENUM_H

/// @file GbmBddEngineEnum.h
/// @brief GbmBddEngineEnum のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmLogic/Bdd.h"
#include "YmLogic/BddMgr.h"
#include "YmLogic/Bool3.h"
#include "RcfNetwork.h"
#include "RcfNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmBddEngineEnum GbmBddEngineEnum.h "GbmBddEngineEnum.h"
/// @brief BDD ベースの GbmEngine
//////////////////////////////////////////////////////////////////////
class GbmBddEngineEnum
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr BddMgr
  GbmBddEngineEnum(BddMgr& mgr);

  /// @brief デストラクタ
  ~GbmBddEngineEnum();


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
  void
  init_vars(const RcfNetwork& network);

  /// @brief 入力値を割り当てて解の候補を求める．
  /// @param[in] network 対象の LUT ネットワーク
  /// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
  /// @param[in] oval 出力の値
  /// @return 結果が空でなければ true を返し，model にその1つを収める．
  bool
  make_bdd(const RcfNetwork& network,
	   ymuint bitpat,
	   const vector<ymuint>& iorder,
	   bool oval);

  /// @brief 結果からモデルを一つ取り出す．
  /// @param[out] model モデル
  void
  get_model(vector<Bool3>& model);

  /// @brief SAT モデルから設定変数の割り当てを取り出す．
  /// @param[in] model SAT モデル
  /// @param[out] conf_bits 設定変数の割り当て
  void
  get_conf_bits(const vector<Bool3>& model,
		vector<bool>& conf_bits) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

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

  // 各 RcfNode の関数を格納する配列
  vector<Bdd> mNodeBddArray;

  // 途中結果を表す BDD
  Bdd mSolution;

  // デバッグフラグ
  bool mDebug;
};

END_NAMESPACE_YM


#endif // GBMBDDENGINEENUM_H
