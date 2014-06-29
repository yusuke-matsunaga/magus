#ifndef SATENGINESINGLE_H
#define SATENGINESINGLE_H

/// @file SatEngineSingle.h
/// @brief SatEngineSingle のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngineSingle SatEngineSingle.h "SatEngineSingle.h"
/// @brief 1つの故障を対象とした CNF を生成する SatEngine
//////////////////////////////////////////////////////////////////////
class SatEngineSingle :
  public SatEngine
{
public:

  /// @brief コンストラクタ
  SatEngineSingle();

  /// @brief デストラクタ
  virtual
  ~SatEngineSingle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプションを設定する．
  void
  set_option(const string& option_str);

  /// @brief テスト生成を行なう．
  /// @param[in] f_tgt 対象の故障
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  run(TpgFault* f_tgt,
      ymuint max_id,
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

  // NEMESIS モード
  bool mNemesis;

  // extected NEMESIS モード
  bool mExtNemesis;

  // TG-GRASP モード
  bool mTgGrasp;

  // extended TG-GRASP モード
  bool mExtTgGrasp;

  // unique sensitization を使う
  bool mUseDominator;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_SATPG

#endif // SATENGINESINGLE_H
