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
  /// @param[in] th_val しきい値
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  SatEngineMulti2(ymuint th_val,
		  const string& sat_type,
		  const string& sat_option,
		  ostream* sat_outp,
		  ymuint max_id,
		  BackTracer& bt,
		  DetectOp& dop,
		  UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~SatEngineMulti2();


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

  // しきい値
  ymuint32 mThVal;

  // 作業用のノードリスト
  vector<TpgNode*> mTmpNodeList;

  // 作業用の配列
  vector<bool> mDone;

  // 処理済みのノードのマーク
  vector<ymuint> mMark;

};

END_NAMESPACE_YM_SATPG

#endif // SATENGINEMULTI2_H
