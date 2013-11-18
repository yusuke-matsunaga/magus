#ifndef GBMENGINE_H
#define GBMENGINE_H

/// @file GbmEngine.h
/// @brief GbmEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "GbmLit.h"
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
  GbmEngine();

  /// @brief デストラクタ
  ~GbmEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief AND ゲートを表す節を追加する．
  /// @param[in] solver SATソルバ
  /// @param[in] input_vars 入力変数のリスト
  /// @param[in] output_var 出力変数
  /// @return 割り当てが矛盾を起こしたら false を返す．
  bool
  make_AND(SatSolver& solver,
	   const vector<GbmLit>& input_vars,
	   GbmLit output_var);

  /// @brief LUT を表す節を追加する．
  /// @param[in] solver SATソルバ
  /// @param[in] input_vars 入力変数のリスト
  /// @param[in] lut_vars LUT変数のリスト
  /// @param[in] output_var 出力変数
  /// @note lut_vars のサイズは input_vars のサイズの指数乗
  /// @return 割り当てが矛盾を起こしたら false を返す．
  bool
  make_LUT(SatSolver& solver,
	   const vector<GbmLit>& input_vars,
	   const vector<GbmLit>& lut_vars,
	   GbmLit output_var);

  /// @brief MUX を表す節を追加する．
  /// @param[in] solver SATソルバ
  /// @param[in] d_vars データ入力変数のリスト
  /// @param[in] s_vars 選択信号変数のリスト
  /// @param[in] output_var 出力変数
  /// @note d_vars のサイズは s_vars のサイズの指数乗
  /// @note 実は make_LUT() と同じことをやっている．
  /// @return 割り当てが矛盾を起こしたら false を返す．
  bool
  make_MUX(SatSolver& solver,
	   const vector<GbmLit>& d_vars,
	   const vector<GbmLit>& s_vars,
	   GbmLit output_var);

};

END_NAMESPACE_YM

#endif // GBMENGINE_H
