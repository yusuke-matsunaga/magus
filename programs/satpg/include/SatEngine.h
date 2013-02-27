#ifndef SATENGINE_H
#define SATENGINE_H

/// @file SatEngine.h
/// @brief SatEngine のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

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

  /// @brief skip モードに設定する．
  /// @param[in] threshold 検出不能故障をスキップするしきい値
  virtual
  void
  set_skip(ymuint32 threshold) = 0;

  /// @brief skip モードを解除する．
  virtual
  void
  clear_skip() = 0;

  /// @brief get_pat フラグを設定する．
  virtual
  void
  set_get_pat(ymuint val) = 0;

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  /// @param[in] max_id ノード番号の最大値 + 1
  /// @param[in] op テスト生成後に呼ばれるファンクター
  virtual
  void
  run(const vector<TpgFault*>& flist,
      ymuint max_id,
      TpgOperator& op) = 0;

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
