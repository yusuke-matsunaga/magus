#ifndef SATENGINESINGLEKDET_H
#define SATENGINESINGLEKDET_H

/// @file SatEngineSingleKDet.h
/// @brief SatEngineSingleKDet のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngineSingleKDet SatEngineSingleKDet.h "SatEngineSingleKDet.h"
/// @brief 1つの故障を対象とした CNF を生成する SatEngine
//////////////////////////////////////////////////////////////////////
class SatEngineSingleKDet :
  public SatEngine
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
  /// @param[in] kdet 多重度
  SatEngineSingleKDet(const string& sat_type,
		      const string& sat_option,
		      ostream* sat_outp,
		      const TpgNetwork& network,
		      BackTracer& bt,
		      DetectOp& dop,
		      UntestOp& uop,
		      ymuint kdet);

  /// @brief デストラクタ
  virtual
  ~SatEngineSingleKDet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] f_tgt 対象の故障
  virtual
  void
  run(TpgFault* f_tgt);

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  virtual
  void
  run(const vector<TpgFault*>& flist);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 多重度
  ymuint mCount;

};

END_NAMESPACE_YM_SATPG

#endif // SATENGINESINGLEKDET_H
