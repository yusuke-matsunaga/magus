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
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  SatEngineMulti(const string& sat_type,
		 const string& sat_option,
		 ostream* sat_outp,
		 ymuint max_id,
		 BackTracer& bt,
		 DetectOp& dop,
		 UntestOp& uop,
		 bool forget);

  /// @brief デストラクタ
  virtual
  ~SatEngineMulti();


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

  // 作業用のノードリスト
  vector<TpgNode*> mTmpNodeList;

  // 作業用の配列
  vector<bool> mDone;

  // forget フラグ
  bool mForget;

};

END_NAMESPACE_YM_SATPG

#endif // SATENGINEMULTI_H
