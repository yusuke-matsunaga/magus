#ifndef GBMENGINE_H
#define GBMENGINE_H

/// @file GbmEngine.h
/// @brief GbmEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "GbmVar.h"
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
  /// @param[in] sat_type SAT-solver の種類を表す文字列
  /// @param[in] sat_opt SAT-solver に渡すオプション文字列
  /// @param[in] sat_log ログの出力用ストリーム
  GbmEngine(const string& sat_type,
	    const string& sat_opt,
	    ostream* sat_log);

  /// @brief デストラクタ
  ~GbmEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief AND ゲートを表す節を追加する．
  /// @param[in] input_vars 入力変数のリスト
  /// @param[in] output_var 出力変数
  bool
  make_AND(const vector<GbmVar>& input_vars,
	   GbmVar output_var);

  /// @brief LUT を表す節を追加する．
  /// @param[in] input_vars 入力変数のリスト
  /// @param[in] lut_vars LUT変数のリスト
  /// @param[in] output_var 出力変数
  /// @note lut_vars のサイズは input_vars のサイズの指数乗
  bool
  make_LUT(const vector<GbmVar>& input_vars,
	   const vector<GbmVar>& lut_vars,
	   GbmVar output_var);

  /// @brief MUX を表す節を追加する．
  /// @param[in] d_vars データ入力変数のリスト
  /// @param[in] s_vars 選択信号変数のリスト
  /// @param[in] output_var 出力変数
  /// @note d_vars のサイズは s_vars のサイズの指数乗
  /// @note 実は make_LUT() と同じことをやっている．
  bool
  make_MUX(const vector<GbmVar>& d_vars,
	   const vector<GbmVar>& s_vars,
	   GbmVar output_var);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT ソルバ
  SatSolver mSolver;

};

END_NAMESPACE_YM

#endif // GBMENGINE_H
