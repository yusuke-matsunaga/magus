#ifndef GBMBDDENGINE_H
#define GBMBDDENGINE_H

/// @file GbmBddEngine.h
/// @brief GbmBddEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmBddEngine GbmBddEngine.h "GbmBddEngine.h"
/// @brief BDD ベースの GbmEngine
//////////////////////////////////////////////////////////////////////
class GbmBddEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] conf_num 設定変数の数
  /// @param[in] input_num 入力数
  /// @param[in] rep 関数の対称変数の代表番号を収める配列
  GbmBddEngine(BddMgr& mgr,
	       ymuint conf_num,
	       ymuint input_num,
	       const vector<ymuint>& rep);

  /// @brief デストラクタ
  ~GbmBddEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 対称性を考慮して初期解を作る．
  void
  init_vars();

  /// @brief 入力値を割り当てて解の候補を求める．
  /// @param[in] network 対象の LUT ネットワーク
  /// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
  /// @param[in] oid 出力のノード番号
  /// @param[in] oval 出力の値
  /// @param[out] model モデル
  /// @return 結果が空でなければ true を返し，model にその1つを収める．
  bool
  make_bdd(const RcfNetwork& network,
	   ymuint bitpat,
	   ymuint oid,
	   bool oval,
	   vector<Bool3>& model);

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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 設定変数番号をキーにして BDD上の変数番号を格納する配列
  vector<VarId> mConfVarArray;

  // 入力数
  ymuint32 mInputNum;

  // 関数の対称性を表す配列
  // mRep[i] に関数の i 番めの入力の等価グループの先行者番号が入る．
  // 自分が等価グループの先頭の場合(singleton も含む)には自分の番号が入る．
  const vector<ymuint>& mRep;

  // 入力順を表す変数の配列
  // i * mInputNum + j 番めの要素は LUT network の i 番めの入力と
  // 関数の j 番めの入力が接続しているときに true となる変数を
  // 格納している．
  vector<VarId> mIorderVarArray;


};

END_NAMESPACE_YM


#endif // GBMBDDENGINE_H
