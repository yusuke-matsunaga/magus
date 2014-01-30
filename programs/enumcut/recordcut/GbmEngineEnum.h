#ifndef GBMENGINEENUM_H
#define GBMENGINEENUM_H

/// @file GbmEngineEnum.h
/// @brief GbmEngineEnum のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmEngineEnum GbmEngineEnum.h "GbmEngineEnum.h"
/// @brief 入力順を固定する GbmEngine
//////////////////////////////////////////////////////////////////////
class GbmEngineEnum :
  public GbmEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] solver SATソルバ
  GbmEngineEnum(SatSolver& solver);

  /// @brief デストラクタ
  ~GbmEngineEnum();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力値を割り当てて CNF 式を作る．
  /// @param[in] network 対象の LUT ネットワーク
  /// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
  /// @param[in] iorder 入力順
  ///　          iorder[pos] に network の pos 番めの入力に対応した
  ///            関数の入力番号が入る．
  /// @param[in] oval 出力の値
  /// @note 結果のCNF式は SAT ソルバに追加される．
  bool
  make_cnf(const RcfNetwork& network,
	   ymuint bit_pat,
	   const vector<ymuint>& iorder,
	   bool oval);

};

END_NAMESPACE_YM

#endif // GBMENGINEENUM_H
