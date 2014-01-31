#ifndef GBMSATENGINEBINARY_H
#define GBMSATENGINEBINARY_H

/// @file GbmSatEngineBinary.h
/// @brief GbmSatEngineBinary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSatEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmSatEngineBinary GbmSatEngineBinary.h "GbmSatEngineBinary.h"
/// @brief 入力順を2値符号化する GbmSatEngine
//////////////////////////////////////////////////////////////////////
class GbmSatEngineBinary :
  public GbmSatEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] solver SATソルバ
  GbmSatEngineBinary(SatSolver& solver);

  /// @brief デストラクタ
  ~GbmSatEngineBinary();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の初期化を行う．
  /// @param[in] network 対象の LUT ネットワーク
  /// @param[in] rep 関数の対称変数の代表番号を収める配列
  ///            rep[pos] に pos 番めの入力の代表番号が入る．
  void
  init_vars(const RcfNetwork& network,
	    const vector<ymuint>& rep);

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

  // 入力順を表す変数の数
  ymuint32 mIorderBitWidth;

  // 入力順を表す変数の配列
  vector<VarId> mIorderVarArray;

};

END_NAMESPACE_YM

#endif // GBMSATENGINEBINARY_H
