#ifndef SATENGINEMULTI_H
#define SATENGINEMULTI_H

/// @file SatEngineMulti.h
/// @brief SatEngineMulti のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngineMulti SatEngineMulti.h "SatEngineMulti.h"
/// @brief 複数の故障の検出に使える CNF 式を生成するタイプの SatEngine
//////////////////////////////////////////////////////////////////////
class SatEngineMulti :
  public SatEngine
{
public:

  /// @brief コンストラクタ
  SatEngineMulti();

  /// @brief デストラクタ
  virtual
  ~SatEngineMulti();


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
      const vector<TpgNode*>& fnode_list,
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

  // 作業用のノードリスト
  vector<TpgNode*> mTmpNodeList;

};

END_NAMESPACE_YM_SATPG

#endif // SATENGINEMULTI_H
