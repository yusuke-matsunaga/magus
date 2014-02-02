#ifndef GBMSATENGINEENUM_H
#define GBMSATENGINEENUM_H

/// @file GbmSatEngineEnum.h
/// @brief GbmSatEngineEnum のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSatEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmSatEngineEnum GbmSatEngineEnum.h "GbmSatEngineEnum.h"
/// @brief 入力順を固定する GbmSatEngine
//////////////////////////////////////////////////////////////////////
class GbmSatEngineEnum :
  public GbmSatEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] solver SATソルバ
  GbmSatEngineEnum(SatSolver& solver);

  /// @brief デストラクタ
  ~GbmSatEngineEnum();


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

#endif // GBMSATENGINEENUM_H
