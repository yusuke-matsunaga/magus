#ifndef SATENGINESINGLE_H
#define SATENGINESINGLE_H

/// @file SatEngineSingle.h
/// @brief SatEngineSingle のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngineSingleBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngineSingle SatEngineSingle.h "SatEngineSingle.h"
/// @brief 1つの故障を対象とした CNF を生成する SatEngine
//////////////////////////////////////////////////////////////////////
class SatEngineSingle :
  public SatEngineSingleBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] network 対象のネットワーク
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  SatEngineSingle(const string& sat_type,
		  const string& sat_option,
		  ostream* sat_outp,
		  const TpgNetwork& network,
		  BackTracer& bt,
		  DetectOp& dop,
		  UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~SatEngineSingle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] f_tgt 対象の故障
  virtual
  void
  run_single(TpgFault* f_tgt);

};

END_NAMESPACE_YM_SATPG

#endif // SATENGINESINGLE_H
