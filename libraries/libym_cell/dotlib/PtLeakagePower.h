#ifndef LIBYM_CELL_DOTLIB_PTLEAKAGEPOWER_H
#define LIBYM_CELL_DOTLIB_PTLEAKAGEPOWER_H

/// @file libym_cell/dotlib/PtLeakagePower.h
/// @brief PtLeakagePower のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtLeakagePower PtLeakagePower.h "PtLeakagePower.h"
/// @brief
//////////////////////////////////////////////////////////////////////
class PtLeakagePower :
  public PtNode
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  PtLeakagePower();

  /// @brief デストラクタ
  virtual
  ~PtLeakagePower();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の数を返す．
  /// @note simple attribute なら常に1
  virtual
  ymuint
  value_num() const;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
  virtual
  const PtValue*
  value(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  // power_level 属性を得る．

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // power_level 属性
  PtValue* mPowerLevel;

  // related_pg_pin 属性
  PtValue* mRelatedPgPin;

  // when 属性
  PtValue* mWhen;

  // value 属性
  PtValue* mValue;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTLEAKAGEPOWER_H
