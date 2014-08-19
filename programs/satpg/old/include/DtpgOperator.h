#ifndef DTPGOPERATOR_H
#define DTPGOPERATOR_H

/// @file DtpgOperator.h
/// @brief DtpgOperator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgOperator DtpgOperator.h "DtpgOperator.h"
/// @brief DtpgSat の処理後に呼ばれるファンクター
//////////////////////////////////////////////////////////////////////
class DtpgOperator
{
public:

  /// @brief テストパタンが見つかった場合に呼ばれる関数
  /// @param[in] f 故障
  /// @param[in] val_list "入力ノードの番号 x 2 + 値" のリスト
  virtual
  void
  set_detected(SaFault* f,
	       const vector<ymuint>& val_list) = 0;

  /// @brief 検出不能のときに呼ばれる関数
  /// @param[in] f 故障
  virtual
  void
  set_untestable(SaFault* f) = 0;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGOPERATOR_H
