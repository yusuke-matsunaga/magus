#ifndef SATENGINESINGLE2_H
#define SATENGINESINGLE2_H

/// @file SatEngineSingle2.h
/// @brief SatEngineSingle2 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngineSingle2 SatEngineSingle2.h "SatEngineSingle2.h"
/// @brief 1つの故障を対象とした CNF を生成する SatEngine
//////////////////////////////////////////////////////////////////////
class SatEngineSingle2 :
  public SatEngine
{
public:

  /// @brief コンストラクタ
  SatEngineSingle2();

  /// @brief デストラクタ
  virtual
  ~SatEngineSingle2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプションを設定する．
  void
  set_option(const string& option_str);

  /// @brief テスト生成を行なう．
  /// @param[in] f_tgt 対象の故障
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  run(TpgNetwork& network,
      TpgFault* f_tgt,
      BackTracer& bt,
      DetectOp& dop,
      UntestOp& uop);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // UopDummy
  UntestOp* mUopDummy;

  // TG-GRASP モード
  bool mTgGrasp;

  // unique sensitization を使う
  bool mUseDominator;

  // PO ごとの dominator を使う．
  bool mUseLocalDominator;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_SATPG

#endif // SATENGINESINGLE2_H
