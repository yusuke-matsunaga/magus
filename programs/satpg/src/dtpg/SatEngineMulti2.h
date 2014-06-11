#ifndef SATENGINEMULTI2_H
#define SATENGINEMULTI2_H

/// @file SatEngineMulti2.h
/// @brief SatEngineMulti2 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngineMulti2 SatEngineMulti2.h "SatEngineMulti2.h"
/// @brief 複数の故障の検出に使える CNF 式を生成するタイプの SatEngine
//////////////////////////////////////////////////////////////////////
class SatEngineMulti2 :
  public SatEngine
{
public:

  /// @brief コンストラクタ
  SatEngineMulti2();

  /// @brief デストラクタ
  virtual
  ~SatEngineMulti2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプションを設定する．
  void
  set_option(const string& option_str);

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  void
  run(const vector<TpgFault*>& flist,
      ymuint max_id,
      BackTracer& bt,
      DetectOp& dop,
      UntestOp& uop);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // TG-GRASP モード
  bool mTgGrasp;

  // EXT-TG-GRASP モード
  bool mExtTgGrasp;

  // unique sensitization を使う
  bool mUseDominator;

  // PO ごとの dominator を使う．
  bool mUseLocalDominator;

  // skip threshold
  ymuint32 mSkipThreshold;

  // 作業用のノードリスト
  vector<TpgNode*> mTmpNodeList;

  // 作業用の配列
  vector<bool> mDone;

};

END_NAMESPACE_YM_SATPG

#endif // SATENGINEMULTI2_H
