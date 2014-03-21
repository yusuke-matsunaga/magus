#ifndef SATENGINE_H
#define SATENGINE_H

/// @file SatEngine.h
/// @brief SatEngine のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

class DtpgSat;

//////////////////////////////////////////////////////////////////////
/// @class SatEngine SatEngine.h "SatEngine.h"
/// @brief SAT を用いたパタン生成器
//////////////////////////////////////////////////////////////////////
class SatEngine
{
public:

  /// @brief デストラクタ
  virtual
  ~SatEngine() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  virtual
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL) = 0;

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  virtual
  void
  run(const vector<TpgFault*>& flist,
      ymuint max_id,
      BackTracer& bt,
      DetectOp& dop,
      UntestOp& uop) = 0;

  /// @brief 統計情報をクリアする．
  virtual
  void
  clear_stats() = 0;

  /// @brief 統計情報を得る．
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  get_stats(DtpgStats& stats) const = 0;

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable) = 0;

};


//////////////////////////////////////////////////////////////////////
// インスタンス生成用の関数
//////////////////////////////////////////////////////////////////////

/// @brief SatEngine の継承クラスを生成する．
extern
SatEngine*
new_SatEngine();

END_NAMESPACE_YM_SATPG

#endif // SATENGINE_H
