#ifndef DTPGDRIVER_H
#define DTPGDRIVER_H

/// @file DtpgDriver.h
/// @brief DtpgDriver のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "DtpgMode.h"
#include "YmNetworks/tgnet.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgDriver DtpgDriver.h "DtpgDriver.h"
/// @brief DTPG を行う基底クラス
//////////////////////////////////////////////////////////////////////
class DtpgDriver
{
public:

  /// @brief デストラクタ
  virtual
  ~DtpgDriver() {}


public:
  //////////////////////////////////////////////////////////////////////
  // パタン生成を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] tpgnetwork 対象のネットワーク
  /// @param[in] mode メインモード
  /// @param[in] po_mode PO分割モード
  /// @param[in] engine DTPG エンジン
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& tgnetwork,
      tDtpgMode mode,
      tDtpgPoMode po_mode,
      DtpgEngine& engine,
      DtpgStats& stats) = 0;

};


/// @brief DtpgDriver のインスタンスを生成する．
extern
DtpgDriver*
new_DtpgDriver();

END_NAMESPACE_YM_SATPG

#endif // DTPGDRIVER_H
