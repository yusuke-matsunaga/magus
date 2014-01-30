#ifndef GBMENGINEBINARY_H
#define GBMENGINEBINARY_H

/// @file GbmEngineBinary.h
/// @brief GbmEngineBinary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmEngineBinary GbmEngineBinary.h "GbmEngineBinary.h"
/// @brief 入力順を2値符号化する GbmEngine
//////////////////////////////////////////////////////////////////////
class GbmEngineBinary :
  public GbmEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] solver SATソルバ
  /// @param[in] node_num ノード数
  /// @param[in] conf_num 設定変数の数
  /// @param[in] input_num 入力数
  /// @param[in] rep 関数の対称変数の代表番号を収める配列
  ///            rep[pos] に pos 番めの入力の代表番号が入る．
  GbmEngineBinary(SatSolver& solver,
		  ymuint node_num,
		  ymuint conf_num,
		  ymuint input_num,
		  const vector<ymuint>& rep);

  /// @brief デストラクタ
  ~GbmEngineBinary();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の初期化を行う．
  /// @param[in] network 対象の LUT ネットワーク
  void
  init_vars(const RcfNetwork& network);

  /// @brief 入力値を割り当てて CNF 式を作る．
  /// @param[in] network 対象の LUT ネットワーク
  /// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
  /// @param[in] oval 出力の値
  /// @note 結果のCNF式は SAT ソルバに追加される．
  bool
  make_cnf(const RcfNetwork& network,
	   ymuint bit_pat,
	   bool oval);

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // 入力の対称性を表す配列
  const vector<ymuint>& mRep;

  // 入力順を表す変数の数
  ymuint32 mIorderBitWidth;

  // 入力順を表す変数の配列
  vector<VarId> mIorderVarArray;

};

END_NAMESPACE_YM

#endif // GBMENGINEBINARY_H
