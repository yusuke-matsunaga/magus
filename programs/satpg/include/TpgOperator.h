#ifndef TPGOPERATOR_H
#define TPGOPERATOR_H

/// @file TpgOperator.h
/// @brief TpgOperator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgOperator TpgOperator.h "TpgOperator.h"
/// @brief Dtpg の処理後に呼ばれるファンクター
//////////////////////////////////////////////////////////////////////
class TpgOperator
{
public:

  /// @brief デストラクタ
  virtual
  ~TpgOperator() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった場合に呼ばれる関数
  /// @param[in] f 故障
  /// @param[in] val_list "入力ノードの番号 x 2 + 値" のリスト
  virtual
  void
  set_detected(TpgFault* f,
	       const vector<ymuint>& val_list) = 0;

  /// @brief 検出不能のときに呼ばれる関数
  /// @param[in] f 故障
  virtual
  void
  set_untestable(TpgFault* f) = 0;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGOPERATOR_H
